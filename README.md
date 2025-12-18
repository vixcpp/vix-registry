# vix-registry

High-performance **package registry engine** built on **Vix.cpp (C++)**.

---

## About

**vix-registry** is a real-world **Vix application** that demonstrates how to build a
high-performance registry service in modern C++.

It showcases:

- Clean application structure on top of **Vix.cpp**
- Modern CMake workflow via `CMakePresets.json`
- Hot-reload and fast iteration
- Optional integration with **Vix ORM**
- A solid foundation for building a production-grade package registry

This project is not just an example — it is the **reference implementation** of a
registry service powered by Vix.

---

## Features

- Simple **HTTP server** powered by `vix::App`
- Modern **C++20** codebase
- Cross-platform build setup via **CMake presets**
- **Hot reload** dev mode: rebuild & restart on save (`vix dev`)
- Optional sanitizers (`VIX_ENABLE_SANITIZERS`)
- Optional **ORM** (`VIX_USE_ORM=ON`)

---

## Project Structure

```csharp
registry/
├── CMakeLists.txt           # si backend en C++/Vix
├── CMakePresets.json        # presets dev/prod
├── Makefile                 # raccourcis (dev, test, run)
├── README.md                # vision + usage
├── CHANGELOG.md             # versions
├── LICENSE
├── .gitignore
├── config/
│   ├── config.dev.json
│   ├── config.prod.json
│   └── config.local.example.json
├── docs/
│   ├── architecture.md
│   ├── api.md               # endpoints REST
│   ├── storage.md           # comment on stocke les paquets
│   ├── security.md          # tokens, scopes, permissions
│   └── roadmap.md
├── migrations/
│   ├── 0001_init_schema.sql
│   ├── 0002_add_tokens.sql
│   └── ...
├── scripts/
│   ├── dev.sh               # lancer serveur en mode dev
│   ├── migrate.sh           # appliquer migrations
│   └── seed.sh              # données de test
├── infra/
│   ├── docker-compose.yml
│   ├── Dockerfile
│   ├── nginx.conf           # reverse proxy si besoin
│   └── k8s/                 # (plus tard)
├── include/
│   └── vix/registry/
│       ├── App.hpp
│       ├── http/
│       │   ├── HttpServer.hpp
│       │   ├── Routes.hpp
│       │   └── Middleware.hpp
│       ├── domain/
│       │   ├── Package.hpp
│       │   ├── Version.hpp
│       │   ├── User.hpp
│       │   ├── Token.hpp
│       │   └── errors.hpp
│       ├── services/
│       │   ├── PackageService.hpp
│       │   ├── VersionService.hpp
│       │   └── AuthService.hpp
│       ├── storage/
│       │   ├── IPackageStorage.hpp
│       │   ├── LocalFileStorage.hpp
│       │   └── S3Storage.hpp          # plus tard
│       └── db/
│           ├── Database.hpp
│           ├── PackageRepository.hpp
│           └── UserRepository.hpp
├── src/
│   ├── main.cpp             # main() ultra minimal → appelle App
│   ├── App.cpp
│   ├── http/
│   │   ├── HttpServer.cpp
│   │   ├── Routes.cpp       # enregistre toutes les routes
│   │   └── Middleware.cpp
│   ├── domain/
│   │   ├── Package.cpp
│   │   ├── Version.cpp
│   │   ├── User.cpp
│   │   └── Token.cpp
│   ├── services/
│   │   ├── PackageService.cpp
│   │   ├── VersionService.cpp
│   │   └── AuthService.cpp
│   ├── storage/
│   │   ├── LocalFileStorage.cpp
│   │   └── S3Storage.cpp        # stub au début
│   └── db/
│       ├── Database.cpp
│       ├── PackageRepository.cpp
│       └── UserRepository.cpp
├── tests/
│   ├── CMakeLists.txt
│   ├── unit/
│   │   ├── test_package.cpp
│   │   ├── test_services.cpp
│   │   └── test_auth.cpp
│   └── integration/
│       ├── test_publish.cpp
│       └── test_resolve.cpp
└── examples/
    ├── curl/
    │   ├── publish_package.http
    │   ├── get_package.http
    │   └── download_version.http
    └── clients/
        ├── vix_client_example.md
        └── python_client_example.py
```

---

## Requirements

- **CMake ≥ 3.20**
- **C++20 compiler**
  - Linux/macOS: Clang ≥ 15 or GCC ≥ 11
  - Windows: Visual Studio 2022 (MSVC 19.3+)
- **Vix.cpp installed** (system install or local build)

---

## Build & Run (Vix CLI)

```bash
vix build        # Configure & build
vix run          # Run (builds if needed)
vix dev          # Hot reload: rebuild & restart on save
```

### Enable ORM (optional)

If your Vix installation exports `vix::orm`, you can enable it like this:

```bash
vix build -D VIX_USE_ORM=ON
vix run   -D VIX_USE_ORM=ON
vix dev   -D VIX_USE_ORM=ON
```

> If `vix::orm` is not available in your install, CMake will fail with a clear error.

---

## Manual CMake (optional)

```bash
cmake --preset dev-ninja
cmake --build --preset dev-ninja
```

### Windows (Visual Studio 2022)

```powershell
cmake --preset dev-msvc
cmake --build --preset dev-msvc
```

---

## Useful Commands

| Command     | Description                            |
| ----------- | -------------------------------------- |
| `vix build` | Build the project                      |
| `vix run`   | Run the project                        |
| `vix dev`   | Hot reload (watch + rebuild + restart) |
| `vix help`  | Show CLI help                          |

---

## Example Output

When the server starts successfully, you’ll see logs like:

```bash
[I] Config loaded from .../config.json
[I] [ThreadPool] started with threads=...
[I] Acceptor initialized on port 8080
[I] Server request timeout set to 5000 ms
```

Open **http://localhost:8080/** in your browser.

---

## About Vix.cpp

[Vix.cpp](https://github.com/vixcpp/vix) is a high-performance, modular C++ backend runtime inspired by **FastAPI**, **Express.js**, and modern tooling.

- Clean routing (`app.get("/", ...)`)
- Modular architecture (`core`, `utils`, `json`, `websocket`, `orm`, ...)
- Simple CMake integration for external apps

---

## License

MIT © [Vix.cpp Authors](https://github.com/vixcpp)
