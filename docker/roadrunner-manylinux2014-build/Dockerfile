# This dockerfile contains commands for a base environment suitable for
# building manylinux pip wheels.
# The default gcc/g++ is 9.3
# Conda is used to manage Python versions.
#
# This dockerfile installs roadrunner dependencies and llvm package
# But does not build roadrunner, which is the subject of another dockerfile

FROM ciaranwelsh/roadrunner-manylinux2014-base

ENV conda /Miniconda3/bin/conda
ENV bashrc /root/.bashrc

RUN $conda init && . $bashrc \
    && conda activate py37 \
    && pip install numpy

RUN git clone https://github.com/sys-bio/roadrunner.git
RUN $conda init && . $bashrc && cd roadrunner && mkdir build && cd build \
    && cmake -DLLVM_INSTALL_PREFIX=/llvm-project/llvm/install-llvm-6.x-rel -DRR_DEPENDENCIES_INSTALL_PREFIX=/libroadrunner-deps/install-libroadrunner-deps -DCMAKE_INSTALL_PREFIX=../install-roadrunner -DBUILD_PYTHON=ON -DBUILD_RR_PLUGINS=ON -DBUILD_TESTS=ON -DPython_ROOT_DIR=/Miniconda3/envs/py37 ..
RUN cd roadrunner/build \
    cmake --build . --target install --config Release -j 12













