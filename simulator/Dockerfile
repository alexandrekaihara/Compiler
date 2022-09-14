FROM i686/ubuntu:latest

COPY setup.sh /home
RUN chmod +x /home/setup.sh
RUN ./home/setup.sh

# For conecting via Open SSL
EXPOSE 22
