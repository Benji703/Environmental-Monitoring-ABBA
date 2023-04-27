#FROM python:3.6.9-alpine
#WORKDIR /code

#RUN apk --update --upgrade add --no-cache  gcc musl-dev jpeg-dev zlib-dev libffi-dev cairo-dev pango-dev gdk-pixbuf-dev

#RUN python -m pip install --upgrade pip
#COPY requirements.txt requirements.txt
#RUN pip install -r requirements.txt
#EXPOSE 1883
#COPY . .
#CMD [ "python3", "publisher.py" ]

FROM python:3.8-slim-buster

COPY requirements.txt /app/requirements.txt

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

RUN pip install --no-cache-dir -r requirements.txt

COPY . /app

CMD ["python", "client.py"]
