FROM python
RUN pip3 --no-cache-dir install flask gunicorn
WORKDIR /srv
COPY jar.py pickle.jpg ./
ENV FLAG="actf{you_got_yourself_out_of_a_pickle}"
EXPOSE 5000
USER nobody
CMD python jar.py
