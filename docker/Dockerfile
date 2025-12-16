# Multi-stage Dockerfile for Card Game (CCG)
# Targets: dev (development) and quality (production-ready)

# =============================================================================
# Stage 1: Build environment
# =============================================================================
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc \
    make \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build

COPY Makefile ./
COPY src/ ./src/
COPY include/ ./include/

# =============================================================================
# Stage 2: Dev build (with debug symbols)
# =============================================================================
FROM builder AS builder-dev

RUN make LINUX=1 USE_SDL2=1 DEBUG=1 all

# =============================================================================
# Stage 3: Quality build (optimized, no debug)
# =============================================================================
FROM builder AS builder-quality

RUN make LINUX=1 USE_SDL2=1 all

# =============================================================================
# Stage 4: Dev runtime image
# =============================================================================
FROM ubuntu:24.04 AS dev

LABEL org.opencontainers.image.source="https://github.com/OWNER/CCG"
LABEL org.opencontainers.image.description="CCG Card Game - Development Build"
LABEL org.opencontainers.image.licenses="MIT"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    libsdl2-2.0-0 \
    libsdl2-image-2.0-0 \
    libsdl2-ttf-2.0-0 \
    gdb \
    valgrind \
    strace \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder-dev /build/bin/card_game /app/card_game

ENTRYPOINT ["/app/card_game"]

# =============================================================================
# Stage 5: Quality runtime image (production)
# =============================================================================
FROM ubuntu:24.04 AS quality

LABEL org.opencontainers.image.source="https://github.com/OWNER/CCG"
LABEL org.opencontainers.image.description="CCG Card Game - Quality Build"
LABEL org.opencontainers.image.licenses="MIT"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    libsdl2-2.0-0 \
    libsdl2-image-2.0-0 \
    libsdl2-ttf-2.0-0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder-quality /build/bin/card_game /app/card_game

ENTRYPOINT ["/app/card_game"]