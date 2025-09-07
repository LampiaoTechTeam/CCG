FROM gcc:12.2 AS build

WORKDIR /app

COPY . .

RUN make clean 
RUN make LINUX=1

FROM alpine:3.14 AS runner

RUN apk add --no-cache gcompat

WORKDIR /app

COPY --from=build /app/bin/card_game /app/card_game

RUN chmod +x /app/card_game

CMD ["/app/card_game"]
