FROM alpine:3.15

# Install funsack
WORKDIR /tmp/
COPY setup_funsack.sh .
RUN ./setup_funsack.sh
RUN rm setup_funsack.sh
