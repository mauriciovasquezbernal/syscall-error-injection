// program.bpf.c

// Kernel types definitions
// Check https://blog.aquasec.com/vmlinux.h-ebpf-programs for more details
#include <vmlinux.h>

// eBPF helpers signatures
// Check https://man7.org/linux/man-pages/man7/bpf-helpers.7.html to learn
// more about different available helpers
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

// Inspektor Gadget macros
#include <gadget/mntns_filter.h>

#define ERRNO_TO_RETURN (-7)

SEC("kprobe/sys_openat")
int BPF_KPROBE(sys_openat)
{
	__u8 comm[TASK_COMM_LEN];

	// Act only on events happening from containers choosen by the user
	if (gadget_should_discard_mntns_id(gadget_get_mntns_id()))
		return 0;

	// And only in the cat process
	// TODO: Currently this is hardcoded, later on this should be implemented by using a hashmap
	bpf_get_current_comm(comm, sizeof(comm));
	if (!__builtin_memcmp(comm, "cat", 3))
		bpf_override_return(ctx, ERRNO_TO_RETURN);

	return 0;
}

char LICENSE[] SEC("license") = "GPL";
