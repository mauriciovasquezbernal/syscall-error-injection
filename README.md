# syscall-error-injection

This is a POC to demomstrate how to inject errors in syscalls by using `bpf_override_return()` and Inspektor Gadget.



### How to build

Note: If you want to use the gadget without modifications, you can use the ghcr.io/mauriciovasquezbernal/gadget/syscall-error-injection image.

The `ig` binary is needed to build the gadget. Follow the instructions on https://www.inspektor-gadget.io/docs/v0.25.1/getting-started/install-linux/#installing-ig to install it.

```bash
$ export IG_EXPERIMENTAL=true
$ sudo -E ig image build . -t syscall-error-injection
INFO[0000] Experimental features enabled
Successfully built ghcr.io/inspektor-gadget/gadget/syscall-error-injection:latest@sha256:895e8d661639b4eea97ca83ce58d594196323c81f8d2dd0f59b94839b355b925
```

### How to run

### Local node

The `ig` binary is needed to run the gadget on the local node. Follow the instructions on https://www.inspektor-gadget.io/docs/v0.25.1/getting-started/install-linux/#installing-ig to install it.

Create a container to block calls on

```bash
$ docker run --name mycontainer --rm -it busybox sh
# cat /dev/null
# comamnd above works fine
```

Run the gadget targeting only mycontainer

> :warning: Be sure you run the gadget only on the container of interest. If not container name is given it'll block the open syscall from the cat process on all containers running in the node.

```bash
$ export IG_EXPERIMENTAL=true
$ sudo -E ig run ghcr.io/mauriciovasquezbernal/gadget/syscall-error-injection -c mycontainer
INFO[0000] Experimental features enabled
INFO[0000] Running. Press Ctrl + C to finish
```

Execute cat on the container

```bash
/ # cat /dev/null
cat: error while loading shared libraries: libm.so.6: cannot open shared object file: Error 7
```

### Kubernetes

Follow https://www.inspektor-gadget.io/docs/v0.25.1/getting-started/install-kubernetes/ to deploy Inspektor Gadget on the cluster. Be sure the `--experimental` flag is using when running the deploy command.

The instructions are very similar to the above, this time other k8s flags like --namespace, --podname, etc. could be used to target the pod.
