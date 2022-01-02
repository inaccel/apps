#syntax=docker/dockerfile:1.2

FROM python AS shell

RUN --mount=source=requirements.txt,target=requirements.txt \
    pip install --no-cache-dir --requirement requirements.txt
