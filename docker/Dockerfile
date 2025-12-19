# Multi-stage Dockerfile for Card Game (CCG)
# Targets: dev (development environment) and quality (production environment)

# =============================================================================
# Dev environment - includes debug tools
# =============================================================================
FROM ubuntu:24.04 AS dev

LABEL org.opencontainers.image.source="https://github.com/OWNER/CCG"
LABEL org.opencontainers.image.description="CCG Card Game - Development Environment"
LABEL org.opencontainers.image.licenses="MIT"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc \
    make \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    gdb \
    valgrind \
    strace \
    file \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# =============================================================================
# Quality environment - minimal production dependencies
# =============================================================================
FROM ubuntu:24.04 AS quality

LABEL org.opencontainers.image.source="https://github.com/OWNER/CCG"
LABEL org.opencontainers.image.description="CCG Card Game - Quality Environment"
LABEL org.opencontainers.image.licenses="MIT"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc \
    make \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app