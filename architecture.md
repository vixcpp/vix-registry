# Architecture — vix-registry

This document describes the **high-level architecture** of **vix-registry**, a high-performance package registry engine built on **Vix.cpp**.

> Scope: architecture and boundaries. For the HTTP contract and endpoints, see `docs/api.md`.

---

## Goals

- **High performance** HTTP registry backend (metadata + downloads).
- **Clean separation** between transport (HTTP), business logic (services), and persistence (storage/db).
- **Extensible** storage backends (local FS, database, object storage like S3 later).
- **Secure by default** authentication with scoped tokens.
- **Production-ready** patterns: config, logging, errors, tests, deployability.

## Non-goals

- A frontend UI (can be added separately).
- A package manager client (CLI/SDK lives outside this repo).
- A build system. The registry stores and serves artifacts, it does not compile them.

---

## High-level Overview

**vix-registry** is structured as a typical Vix application:

- **HTTP Layer**: routes, middleware, request/response mapping.
- **Domain Layer**: pure models (Package, Version, User, Token) and validation rules.
- **Services Layer**: business rules and orchestration.
- **Storage Layer**: persistence abstraction (FS/DB/object storage).
- **DB Layer** (optional): repositories and database wiring when `VIX_USE_ORM=ON`.

### System diagram

```text
Client (CLI / SDK / CI)
        |
        | HTTP (JSON + artifact downloads)
        v
  HTTP Layer (Routes + Middleware)
        |
        v
  Services (Auth, Package, Version)
        |
        v
  Storage Abstraction (interfaces)
        |
        +--> Local FS Storage (dev / simple deploy)
        +--> DB Storage (prod)        [optional]
        +--> Object Storage (S3)      [future]
```

---

## Repository Layout (conceptual)

The repo is organized into **clear domains**, designed to keep compile boundaries clean:

- `include/vix/registry/**` — public headers (core types, interfaces)
- `src/**` — implementations
- `docs/**` — architecture + API + security + roadmap
- `migrations/**` — schema migrations (when DB enabled)
- `infra/**` — container and deployment assets (optional)

---

## Layer Responsibilities

### 1) HTTP layer (`http/`)

**Purpose**: translate HTTP into domain/service calls and return consistent responses.

Components:

- `Routes`  
  Registers endpoints and maps them to service methods.

- `Middleware`

  - request id / trace id injection
  - auth token extraction
  - JSON parsing & size limits
  - rate limiting hooks (future)
  - standardized error mapping

- `HttpServer`  
  Owns server lifecycle (port, thread pool config, timeouts).

**Rules**:

- The HTTP layer should not contain business rules beyond basic request validation.
- Never talk directly to DB or files — always call services/storage.

---

### 2) Domain layer (`domain/`)

**Purpose**: pure, framework-free data structures + validation rules.

Core entities:

- `Package`

  - unique name
  - owner
  - visibility (public/private)
  - created/updated timestamps

- `Version`

  - semver
  - artifact location (dist URL or storage key)
  - integrity hash (sha256)
  - published timestamp
  - yank state (optional)

- `User`

  - id, username/email
  - created timestamp

- `Token`
  - hashed token
  - user id
  - scopes (publish/read/admin)
  - expiry and revocation state

**Rules**:

- Domain is **pure**: no HTTP, no DB, no Vix types.
- Validation lives here (e.g., semver parsing, name rules).

---

### 3) Services layer (`services/`)

**Purpose**: implement business rules and workflows.

Services:

- `AuthService`

  - validate token (hash + lookup)
  - enforce scopes
  - optional token creation/revocation flows

- `PackageService`

  - create package
  - get package metadata
  - list packages (future)
  - permissions: owner/admin required for mutations

- `VersionService`
  - publish version (semver uniqueness)
  - resolve version
  - list versions
  - yank/un-yank (future)

**Rules**:

- Services orchestrate multiple storage operations (transactional when DB enabled).
- Services return **domain objects** or DTOs; HTTP formats them.

---

### 4) Storage layer (`storage/`)

**Purpose**: hide persistence details behind stable interfaces.

Why it matters:

- lets the registry run in **memory/local FS** for dev
- while supporting **DB + object storage** for production

Suggested interfaces:

- `IPackageStorage`

  - get/create package
  - existence checks

- `IVersionStorage`

  - publish version
  - list/resolve versions
  - yank operations

- `IAuthStorage`
  - token lookup
  - user lookup (optional)

Concrete backends:

- `LocalFileStorage`  
  JSON metadata + file-based artifacts (simple deploy, dev friendly).

- `DbStorage`  
  Uses DB repositories (ORM) for metadata. Artifacts may still live in FS/S3.

- `S3Storage` _(future)_  
  Stores artifacts in object storage; metadata stays in DB.

---

## Request Flow Examples

### A) Fetch package metadata (read path)

```text
GET /v1/packages/{name}
  -> Routes::getPackage(name)
  -> PackageService::get(name)
  -> IPackageStorage::get(name)
  -> return JSON response (consistent envelope)
```

### B) Publish a version (write path)

```text
POST /v1/packages/{name}/versions
  -> Middleware authenticates token + scope "publish"
  -> Routes::publishVersion(...)
  -> VersionService::publish(...)
      - validate semver
      - ensure package exists
      - write metadata (DB or file store)
      - store artifact reference + sha256
  -> return created version payload
```

---

## Data & Persistence Model

vix-registry typically separates:

1. **Metadata** (packages, versions, owners, tokens)
2. **Artifacts** (tarball/zip/binary blobs)

This enables flexible deployments:

- small/simple: metadata + artifacts on local disk
- standard production: metadata in DB, artifacts on disk or S3
- enterprise: DB + S3 + CDN

---

## Authentication & Authorization

Token-based auth is recommended:

- Clients send `Authorization: Bearer <token>`
- Tokens are stored **hashed** (never store raw tokens)
- Each token has a set of **scopes**:
  - `read` — fetch metadata and download artifacts
  - `publish` — publish new versions
  - `admin` — yanking, deleting, managing packages/tokens

Middleware extracts token and attaches an **AuthContext** to the request context.

---

## Configuration

Configuration is loaded from:

- `config/config.dev.json`
- `config/config.prod.json`
- optional `config.local.json` (ignored by git)

Typical fields:

- `port`
- `log_level`
- `request_timeout_ms`
- `max_body_bytes`
- `storage_backend` (fs/db)
- `db` (host, user, name, pool)
- `artifacts_dir`

---

## Error Handling

Errors should be represented as typed domain errors, then mapped to HTTP:

- `ValidationError` -> 400
- `AuthError` -> 401/403
- `NotFoundError` -> 404
- `ConflictError` -> 409
- `InternalError` -> 500

Response envelope example:

```json
{
  "ok": false,
  "error": {
    "code": "VALIDATION_ERROR",
    "message": "Invalid semver"
  }
}
```

---

## Logging & Observability

Recommended logging principles:

- include `request_id` / `trace_id` in every log line
- log errors with structured fields (method, path, status, latency)
- expose `/health` for liveness

Future:

- `/metrics` (Prometheus)
- structured JSON logs

---

## Testing Strategy

- **Unit tests** for:

  - domain validation (name rules, semver parsing)
  - service logic (publish rules, permissions)

- **Integration tests** for:
  - publish and resolve flows via HTTP
  - storage backends (FS/DB)

---

## Deployment Model

Typical deployment patterns:

### 1) Single node (simple)

- vix-registry + local disk storage
- optional Nginx reverse proxy

### 2) Production baseline

- vix-registry (stateless)
- DB for metadata
- artifacts on shared volume or S3
- CDN in front of artifact downloads

### 3) Kubernetes

- multiple replicas of vix-registry
- external DB + S3
- ingress + autoscaling

---

## Roadmap (architecture-level)

- **v0**: local storage backend, basic auth, publish/read API
- **v1**: DB backend + migrations, scoped tokens, yanking, pagination
- **v2**: S3 artifacts, CDN integration, metrics, rate limits
- **vNext**: multi-tenant registries, orgs/teams, web UI

---

## Design Principles

- Keep **domain pure** and framework-free.
- Keep services deterministic and testable.
- Use storage interfaces to avoid lock-in.
- Prefer **explicit** behavior over magic.
- Optimize for **observability** and safe operations.
