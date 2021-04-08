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

RUN cd llvm-project/llvm \
    && git checkout d359f2096850c68b708bc25a7baca4282945949f \
    && mkdir build && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install-llvm-6.x-rel -DPython3_EXECUTABLE=/Miniconda3/bin/python ..

RUN cd llvm-project/llvm/build \
    && cmake --build . --target install --config Release -j 12

RUN git clone https://github.com/sys-bio/libroadrunner-deps.git --recurse-submodules
RUN cd libroadrunner-deps && mkdir build && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX=../install-libroadrunner-deps -DCMAKE_BUILD_TYPE=Release .. \
    && cmake --build . --target install --config Release -j 12