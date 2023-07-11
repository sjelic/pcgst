FROM ubuntu:latest
RUN apt-get update && apt-get install -y locales build-essential wget git unzip cmake
ENV LANG en_US.utf8




WORKDIR /opt
RUN wget https://packages.gurobi.com/10.0/gurobi10.0.2_linux64.tar.gz && tar xvfz gurobi10.0.2_linux64.tar.gz
ENV GUROBI_HOME "/opt/gurobi1002/linux64"
ENV PATH "${PATH}:${GUROBI_HOME}/bin"
ENV LD_LIBRARY_PATH "${GUROBI_HOME}/lib"
ENV GRB_LICENSE_FILE "/workspaces/workspace/praksa/price_collecting_gst/gurobi.lic"

RUN wget http://lemon.cs.elte.hu/pub/sources/lemon-1.3.1.zip
RUN unzip lemon-1.3.1.zip
RUN cd lemon-1.3.1 && mkdir build && cd build && cmake .. && make && make install 
# RUN cp -r ./lemon-1.3.1/lemon /workspaces/workspace/praksa/price_collecting_gst/.
# RUN rm lemon-1.3.1.zip

WORKDIR /opt/gurobi1002/linux64/src/build
RUN make && cp libgurobi_c++.a ../../lib/.
RUN rm /opt/gurobi10.0.2_linux64.tar.gz

CMD ['/bin/bash']