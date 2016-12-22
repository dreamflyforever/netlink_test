#include <linux/module.h>
#include <linux/netlink.h>
#include <net/netlink.h>
#include <net/net_namespace.h>

#define NETLINK_TEST 31

#define NLMSG_SETECHO 0x11
#define NLMSG_GETECHO 0x12

static struct sock *sk;
static void nl_custom_data_ready(struct sk_buff *skb);

int __init nl_custom_init(void)
{
	struct netlink_kernel_cfg nlcfg = {
		.input = nl_custom_data_ready,
	};
	sk = netlink_kernel_create(&init_net, NETLINK_TEST, &nlcfg);
	printk(KERN_INFO "initialed ok!\n");
	if (!sk) {
		printk(KERN_INFO "netlink create error!\n");
	}

	return 0;
}

void __exit nl_custom_exit(void)
{
	printk(KERN_INFO "existing...\n");
	netlink_kernel_release(sk);
}

static void nl_custom_data_ready(struct sk_buff *skb)
{
	struct nlmsghdr *nlh;
	void *payload;
	struct sk_buff *out_skb;
	void *out_payload;
	struct nlmsghdr *out_nlh;

 	/*with padding, but ok for echo */
	int payload_len;
 	nlh = nlmsg_hdr(skb);
	switch(nlh->nlmsg_type) {
	case NLMSG_SETECHO:
		break;
	case NLMSG_GETECHO:
		payload = nlmsg_data(nlh);
		payload_len = nlmsg_len(nlh);
		printk(KERN_INFO "payload_len = %d\n", payload_len);
		printk(KERN_INFO "Recievid: %s, From: %d\n", (char *)payload, nlh->nlmsg_pid);
		out_skb = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL); 

		if (!out_skb) goto failure;

		out_nlh = nlmsg_put(out_skb, 0, 0, NLMSG_SETECHO, payload_len, 0);

		if (!out_nlh) goto failure;

		out_payload = nlmsg_data(out_nlh);
		strcpy(out_payload, "[from kernel]:");
		strcat(out_payload, payload);
		nlmsg_unicast(sk, out_skb, nlh->nlmsg_pid);
		break;
	default:
		printk(KERN_INFO "Unknow msgtype recieved!\n");
	}

	return ;

failure:
	printk(KERN_INFO " failed in fun dataready!\n");
}

module_init(nl_custom_init);
module_exit(nl_custom_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("a simple example for custom netlink protocal family");
MODULE_AUTHOR("Jim");
