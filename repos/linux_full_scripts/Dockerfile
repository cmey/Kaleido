# CLion remote docker environment (How to build docker container, run and stop it)
#
# Build and run:
#   docker build -t jonmmease/kaleido-builder:0.6 -f Dockerfile .
#
# stop:
#   docker stop orca-next
# 
# ssh credentials (test user):
#   user@password

FROM jonmmease/chromium-builder:0.6


# Fetch chromium source tree
RUN \
    cd / \
    && fetch --nohooks chromium
    
# Configure gclient to get windows and linux dependencies
# RUN \
#     echo '\ntarget_os = ["linux", "win"]\n' >> /.gclient \
#     && cat /.gclient

# Checkout chromium revision
RUN \
    cd /src \
    && git checkout tags/$CHROMIUM_TAG \
    && gclient sync -D --force --reset
    
RUN gclient runhooks
    
# Remove .git directory to save space
RUN rm -r /src/.git

# Build create linux out directory
RUN \
    cd /src \
    && mkdir -p out/Kaleido_linux \
    && echo 'import("//build/args/headless.gn")' > out/Kaleido_linux/args.gn \
    && echo 'enable_nacl=false' >> out/Kaleido_linux/args.gn \
    && echo 'is_component_build=false' >> out/Kaleido_linux/args.gn \
    && echo 'symbol_level=0' >> out/Kaleido_linux/args.gn \
    && echo 'blink_symbol_level=0' >> out/Kaleido_linux/args.gn \
    && echo 'is_debug=false' >> out/Kaleido_linux/args.gn


# Add Kaleido executable
RUN echo ' \
executable("kaleido") { \
  sources = [ "app/kaleido.cc" ]  \
  deps = [  \
    ":headless_shell_lib",  \
    "//skia", \
  ]  \
} \
' >> /src/headless/BUILD.gn

    
# Perform build using headless_example. This will make sure we have all of the 
# dependencies that Kaleido needs precompiled.
RUN \
    cd /src \
    && gn gen out/Kaleido_linux \
    && ninja -C out/Kaleido_linux -j 20 headless_example
    