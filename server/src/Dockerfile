FROM ubuntu:20.04
RUN mkdir /src
RUN mkdir /var/log/erss
WORKDIR /src
RUN apt-get update && apt-get install -y g++ make
COPY . /src
RUN make
