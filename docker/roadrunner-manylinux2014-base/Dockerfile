# This dockerfile contains commands for a base environment suitable for
# building manylinux pip wheels.
# The default gcc/g++ is 9.3
# Conda is used to manage Python versions.
#
# This dockerfile installs roadrunner dependencies and llvm package
# But does not build roadrunner, which is the subject of another dockerfile

FROM quay.io/pypa/manylinux2014_x86_64
RUN yum install -y wget
RUN wget https://repo.anaconda.com/miniconda/Miniconda3-py39_4.9.2-Linux-x86_64.sh
RUN bash Miniconda3-py39_4.9.2-Linux-x86_64.sh -b -p /Miniconda3

RUN /Miniconda3/bin/conda create -y --name py37 python=3.7 pytest
RUN /Miniconda3/bin/conda create -y --name py38 python=3.8 pytest
RUN /Miniconda3/bin/conda create -y --name py39 python=3.9 pytest

# note, sys-bio/llvm-6.x did not compile on centos due to tblgen error
#  the original project seems to compile
RUN git clone https://github.com/llvm/llvm-project.git

RUN /Miniconda3/bin/conda init && bash ~/.bashrc && . ~/.bashrc
RUN pwd
RUN git clone https://github.com/sys-bio/libroadrunner-deps.git --recurse-submodules
RUN cd libroadrunner-deps && mkdir build && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX=../install-libroadrunner-deps -DCMAKE_BUILD_TYPE=Release .. \
    && cmake --build . --target install --config Release -j 12


RUN yum install -y pcre-devel.x86_64

ENV swig_source_dir /swig-4.0.2
ENV swig_install_dir /swig-4.0.2/install-swig-4.0.2

RUN  mkdir -p $swig_source_dir && wget -q https://sourceforge.net/projects/swig/files/swig/swig-4.0.2/swig-4.0.2.tar.gz
RUN  tar -xf swig-4.0.2.tar.gz
RUN  cd $swig_source_dir && ./configure --prefix=$swig_install_dir
RUN  cd $swig_source_dir && make -j 12 && make install

RUN pwd
RUN git clone https://github.com/sys-bio/llvm-6.x.git \
    && cd llvm-6.x \
    && git checkout 3c62217a742a56807c8ef693bd2dccbc4a73966c
RUN cd /llvm-6.x/llvm && mkdir build && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX=/llvm-6.x/install-llvm-6.x -DCMAKE_BUILD_TYPE=Release ..
RUN cd /llvm-6.x/llvm/build \
    && cmake --build . --target install --config Release -j 12

RUN yum install -y ncurses-devel
RUN git config --global core.editor nano && git config --global pager.branch false

# build the latest gcc (11.1.0), which should keep us going for a while
# instructions: https://linuxhostsupport.com/blog/how-to-install-gcc-on-centos-7/
RUN wget http://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-11.1.0/gcc-11.1.0.tar.gz
RUN tar zxf gcc-11.1.0.tar.gz && yum -y install bzip2
RUN cd gcc-11.1.0 && ./contrib/download_prerequisites
RUN cd gcc-11.1.0 && mkdir build && cd build && ../configure --prefix=/usr/local/gcc-11.1.0 --disable-multilib
RUN cd gcc-11.1.0/build && make -j 12
RUN cd gcc-11.1.0/build && make install

RUN yum install -y nano
#RUN echo -e "export PATH=\"/usr/local/gcc-11.1.0/bin:$PATH\"" >> ~/.bashrc \
#    && echo -e "export LD_LIBRARY_PATH=\"/usr/local/gcc-11.1.0/lib:$LD_LIBRARY_PATH\"" >> ~/.bashrc

# some additional commands to enable docker integration with CLION
# which is a helpful tool for debugging manylinux builds
# see https://www.jetbrains.com/help/clion/docker.html
# and https://blog.jetbrains.com/clion/2020/01/using-docker-with-clion/#using-the-remote-development-workflow-with-docker
RUN yum install -y openssh-server openssh-clients
RUN ssh-keygen -A

RUN ( \
    echo 'LogLevel DEBUG2'; \
    echo 'PermitRootLogin yes'; \
    echo 'PasswordAuthentication yes'; \
    echo 'Subsystem sftp /usr/libexec/openssh/sftp-server'; \
  ) > /etc/ssh/sshd_config_test_clion

RUN useradd -m user \
  && yes password | passwd user
RUN yum install -y initscripts
#CMD ["/usr/sbin/sshd", "-D", "-e", "-f", "/etc/ssh/sshd_config_test_clion"]








