//内核主命令码
const IPC_MIAN_IPC_KERNEL:uint=0;//内核命令

//内核子命令码
const IPC_SUB_IPC_CLIENT_CONNECT:uint=1;//连接通知
const IPC_SUB_IPC_SERVER_ACCEPT:uint=2;//应答通知
const IPC_SUB_IPC_CLIENT_CLOSE:uint=3;//关闭通知