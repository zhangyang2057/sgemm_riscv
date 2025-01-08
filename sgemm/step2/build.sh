#!/bin/bash

export PATH=$PATH:/home/zhangyang/workspace/k230_conan2_test/nncase/toolchains/Xuantie-900-gcc-linux-6.6.0-glibc-x86_64-V2.10.1/bin
make clean
make
cp *.x /home/share/nfsroot/k230/k230_llm/sgemm_riscv/

