FROM ubuntu:20.04
ENV TZ=America/New_York
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt-get -y update && apt-get install -y
RUN apt-get -y install g++ make postgresql libpq-dev libpqxx-dev
RUN mkdir /servercode
WORKDIR /servercode
COPY . /servercode
CMD ["chmod","+x","run.sh"]