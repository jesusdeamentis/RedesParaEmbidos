#include "tcpecho.h"



#include "lwip/opt.h"



#if LWIP_NETCONN



#include "lwip/sys.h"
#include "lwip/api.h"
#include "aes.h"
/*-----------------------------------------------------------------------------------*/
static void
tcpecho_thread(void *arg)
{
	struct netconn *conn, *newconn;
	err_t err;
	LWIP_UNUSED_ARG(arg);
	ip4_addr_t netif_Servipaddr;

	IP4_ADDR(&netif_Servipaddr, 192, 168, 0, 105);



/* Create a new connection identifier. */
/* Bind connection to well known port number 7. */
	#if LWIP_IPV6
	conn = netconn_new(NETCONN_TCP_IPV6);
	netconn_bind(conn, IP6_ADDR_ANY, 7);
	#else /* LWIP_IPV6 */
	conn = netconn_new(NETCONN_TCP);
//netconn_bind(conn, IP_ADDR_ANY, 7);
	#endif /* LWIP_IPV6 */
	LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);



/* Tell connection to go into listening mode. */
//netconn_listen(conn);



	while (1) {



/* Grab new connection. */
//err = netconn_accept(conn, &newconn);
/*printf("accepted new connection %p\n", newconn);*/
/* Process the new connection. */



		err = netconn_connect(conn, &netif_Servipaddr, 7);
		if (err == ERR_OK) {
			uint8_t key[16] = { (uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, (uint8_t) 0x16, (uint8_t) 0x28, (uint8_t) 0xae, (uint8_t) 0xd2,
					(uint8_t) 0xa6, (uint8_t) 0xab, (uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, (uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, (uint8_t) 0x3c };
			struct netbuf *buf;
			void *data;
			u8_t *Rx_msg;
			u16_t len;
			u8_t Tx_msg[] = "Como serverxxxxx";
			u8_t Tx_msg1[] = "Dia de hooyxxxx";
			u8_t Tx_msg2[] = "Que hora esxxxx";
			u8_t Tx_msg3[] = "Me ama bebexxxx";
			u8_t i;

			printf("Cliente:\n ");
			for(i=0;i<16;i++){
				printf("%c",Tx_msg[i]);
			}
			printf("\n ");
			for(i=0;i<16;i++){
				printf("%c",Tx_msg1[i]);
			}
			printf("\n ");
			for(i=0;i<16;i++){
				printf("%c",Tx_msg2[i]);
			}
			printf("\n ");
			for(i=0;i<16;i++){
				printf("%c",Tx_msg3[i]);
			}
			printf("\n ");
			struct AES_ctx ctx;
			struct AES_ctx ctx1;
			struct AES_ctx ctx2;
			struct AES_ctx ctx3;
			AES_init_ctx(&ctx, key);
			AES_init_ctx(&ctx1, key);
			AES_init_ctx(&ctx2, key);
			AES_init_ctx(&ctx3, key);

			for (i = 0; i < 1; ++i)
			{
				AES_ECB_encrypt(&ctx, Tx_msg + (i * 11));
			}
			for (i = 0; i < 1; ++i)
			{
				AES_ECB_encrypt(&ctx1, Tx_msg1 + (i * 16));
			}
			for (i = 0; i < 1; ++i)
			{
				AES_ECB_encrypt(&ctx2, Tx_msg2 + (i * 16));
			}
			for (i = 0; i < 1; ++i)
			{
				AES_ECB_encrypt(&ctx3, Tx_msg3 + (i * 16));
			}
			printf("Cliente Encriptado:\n ");
			for(i=0;i<16;i++){
				printf("%c",Tx_msg[i]);
			}
			printf("\n ");
			for(i=0;i<16;i++){
				printf("%c",Tx_msg1[i]);
			}
			printf("\n ");
			for(i=0;i<16;i++){
				printf("%c",Tx_msg2[i]);
			}
			printf("\n ");
			for(i=0;i<16;i++){
				printf("%c",Tx_msg3[i]);
			}
			printf("\n ");

			netconn_write(conn, (void*)Tx_msg, (sizeof(Tx_msg)/sizeof(Tx_msg[0])), NETCONN_COPY);
			netconn_write(conn, (void*)Tx_msg1, (sizeof(Tx_msg1)/sizeof(Tx_msg1[0])), NETCONN_COPY);
			netconn_write(conn, (void*)Tx_msg2, (sizeof(Tx_msg2)/sizeof(Tx_msg2[0])), NETCONN_COPY);
			netconn_write(conn, (void*)Tx_msg3, (sizeof(Tx_msg3)/sizeof(Tx_msg3[0])), NETCONN_COPY);


			while ((err = netconn_recv(conn, &buf)) == ERR_OK) {
/*printf("Recved\n");*/
				do {
					netbuf_data(buf, &data, &len);
//err = netconn_write(newconn, data, len, NETCONN_COPY);
					#if 0
					if (err != ERR_OK) {
						printf("tcpecho: netconn_write: error \"%s\"\n", lwip_strerr(err));
					}
					#endif
				}
				while (netbuf_next(buf) >= 0);
				netbuf_delete(buf);
				Rx_msg = (u8_t*)data;
				//struct AES_ctx ctx4;
				//AES_init_ctx(&ctx4, key);
				printf("\n");
				for(i=0;i<16;i++){
					printf("%c",Rx_msg[i]);
				}
				/*for (i = 0; i < 1; ++i){
					AES_ECB_decrypt(&ctx, Rx_msg + (i * 12));
				}
				printf("\n");
				for(i=0;i<len;i++){
					printf("%c",Rx_msg[i]);
				}*/

				/*
				printf("Encriptado Server:\n");
				for(i=0;i<16;i++){
					printf("%.2x",Rx_msg[i]);
				}
				for (i = 0; i < 3; ++i){
					AES_ECB_decrypt(&ctx, Rx_msg + (i));
				}
				printf("\n");
				printf("Desencriptado Server:\n");
				for(i=0;i<16;i++){
					printf("%.2x",Rx_msg[i]);
				}*/

			}
/*printf("Got EOF, looping\n");*/
/* Close connection and discard connection identifier. */
			netconn_close(newconn);
			netconn_delete(newconn);
		}
	}
}
/*-----------------------------------------------------------------------------------*/
void
tcpecho_init(void)
{
sys_thread_new("tcpecho_thread", tcpecho_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/



#endif /* LWIP_NETCONN */
