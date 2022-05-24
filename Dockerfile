FROM ubuntu:20.04

RUN apt update 
RUN apt install -y --no-install-recommends tzdata
RUN apt install -y libboost-all-dev cmake g++ git
WORKDIR /opt
RUN git clone https://github.com/Geant4/geant4.git
RUN mkdir -p /opt/geant4/build
WORKDIR /opt/geant4/build
RUN cmake -DGEANT4_BUILD_MULTITHREADED=ON -DGEANT4_INSTALL_DATA=ON -DGEANT4_INSTALL_PREFIX=/usr/local -DGEANT4_USE_SYSTEM_EXPAT=OFF .. && \ 
    cmake --build . -- -j `nproc` && \
    cmake --build . --target install

WORKDIR /work
COPY . /work
RUN mkdir -p build
WORKDIR /work/build
RUN cmake .. && \
    cmake --build . -- -j `nproc`
RUN chmod +x /work/entry_point.sh
WORKDIR /output
ENTRYPOINT ["/work/entry_point.sh"]
CMD ["/bin/bash"]