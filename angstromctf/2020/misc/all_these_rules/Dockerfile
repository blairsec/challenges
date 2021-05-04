FROM ubuntu:18.04

WORKDIR /

RUN apt-get update
RUN apt-get install -y openssh-server

RUN mkdir -p /var/run/sshd -m 0755
RUN echo "PermitEmptyPasswords yes" >> /etc/ssh/sshd_config
RUN echo "ForceCommand rbash" >> /etc/ssh/sshd_config
RUN echo "ssh" >> /etc/securetty
EXPOSE 22

RUN useradd -m chall -s /bin/rbash
RUN passwd -d chall

RUN mkdir -p /usr/share/rbin -m 0755
RUN ln -s /usr/bin/tee /usr/share/rbin/tee

# RUN echo "* hard nproc 100" >> /etc/security/limits.conf
RUN echo "source ~/.restrictions" >> /home/chall/.bashrc
RUN mkdir /flag -m 755
COPY flag.txt /flag/flag.txt
COPY print_flag /flag/print_flag
RUN groupadd flag
RUN chgrp -R flag /flag
RUN chmod 440 /flag/flag.txt
RUN chmod 2555 /flag/print_flag
COPY bash_profile /home/chall/.bash_profile
COPY restrictions /home/chall/.restrictions
RUN chown -R root:root /home/chall
RUN chmod -R 555 /home/chall

LABEL options='{"ports": {"22/tcp": 20209}, "mem_limit": "512m"}'

CMD ["/usr/sbin/sshd", "-D"]