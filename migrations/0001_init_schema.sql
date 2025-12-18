-- migrations/0001_init_schema.sql
-- vix Registry - initial schema (users, packages, versions)

CREATE TABLE IF NOT EXISTS users (
  id            BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  username      VARCHAR(50)  NOT NULL,
  email         VARCHAR(190) NOT NULL,
  password_hash VARCHAR(255) NOT NULL,

  created_at    TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  updated_at    TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,

  PRIMARY KEY (id),
  UNIQUE KEY uk_users_username (username),
  UNIQUE KEY uk_users_email (email)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;


CREATE TABLE IF NOT EXISTS packages (
  id            BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  owner_user_id BIGINT UNSIGNED NOT NULL,

  name          VARCHAR(120) NOT NULL,
  description   TEXT NULL,
  visibility    ENUM('public','private') NOT NULL DEFAULT 'public',

  created_at    TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  updated_at    TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,

  PRIMARY KEY (id),

  -- un package est unique par owner + name
  UNIQUE KEY uk_packages_owner_name (owner_user_id, name),

  KEY idx_packages_name (name),
  KEY idx_packages_owner (owner_user_id),

  CONSTRAINT fk_packages_owner
    FOREIGN KEY (owner_user_id) REFERENCES users(id)
    ON DELETE CASCADE
    ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;


CREATE TABLE IF NOT EXISTS versions (
  id           BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  package_id   BIGINT UNSIGNED NOT NULL,

  semver       VARCHAR(64)  NOT NULL,   -- ex: 1.2.3, 1.2.3-alpha.1
  artifact_path VARCHAR(512) NOT NULL,  -- chemin storage (local/s3)
  sha256       CHAR(64)     NOT NULL,
  size_bytes   BIGINT UNSIGNED NOT NULL DEFAULT 0,

  yanked       TINYINT(1) NOT NULL DEFAULT 0,
  created_at   TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,

  PRIMARY KEY (id),

  -- une version unique par package + semver
  UNIQUE KEY uk_versions_package_semver (package_id, semver),

  KEY idx_versions_package (package_id),
  KEY idx_versions_semver (semver),
  KEY idx_versions_created_at (created_at),

  CONSTRAINT fk_versions_package
    FOREIGN KEY (package_id) REFERENCES packages(id)
    ON DELETE CASCADE
    ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
