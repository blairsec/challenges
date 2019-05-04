FROM python:2-slim

ADD slots/ /slots/
WORKDIR /slots/

EXPOSE 3002

LABEL options='{"ports": {"3002/tcp": 18004}}'

CMD ["python", "slots.py"]
