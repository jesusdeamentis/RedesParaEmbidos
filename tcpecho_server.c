/*
* Copyright (c) 2001-2003 Swedish Institute of Computer Science.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
* 3. The name of the author may not be used to endorse or promote products
* derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
* SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
*
* This file is part of the lwIP TCP/IP stack.
*
* Author: Adam Dunkels <adam@sics.se>
*
*/
#include "tcpecho.h"



#include "lwip/opt.h"



#if LWIP_NETCONN



#include "lwip/sys.h"
#include "lwip/api.h"

#include "aes.h"
/*-----------------------------------------------------------------------------------*/


static void
tcpecho_thread(void *arg){
	struct netconn *conn, *newconn;
	err_t err;
	LWIP_UNUSED_ARG(arg);



/* Create a new connection identifier. */
/* Bind connection to well known port number 7. */
	#if LWIP_IPV6
	conn = netconn_new(NETCONN_TCP_IPV6);
	netconn_bind(conn, IP6_ADDR_ANY, 7);
	#else /* LWIP_IPV6 */
	conn = netconn_new(NETCONN_TCP);
	netconn_bind(conn, IP_ADDR_ANY, 7);
	#endif /* LWIP_IPV6 */
	LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);



/* Tell connection to go into listening mode. */
	netconn_listen(conn);



	while (1) {



/* Grab new connection. */
		err = netconn_accept(conn, &newconn);
/*printf("accepted new connection %p\n", newconn);*/
/* Process the new connection. */



		if (err == ERR_OK) {
			struct netbuf *buf;
			void *data;
			u8_t *Rx_msg;
			u16_t len;
			u8_t *Tx_msg;
			uint8_t key[16] = { (uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, (uint8_t) 0x16, (uint8_t) 0x28, (uint8_t) 0xae, (uint8_t) 0xd2, (uint8_t) 0xa6, (uint8_t) 0xab, (uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, (uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, (uint8_t) 0x3c };
			u8_t i;
			u8_t f_txt[64];



			while ((err = netconn_recv(newconn, &buf)) == ERR_OK) {
/*printf("Received\n");*/

				do {
					netbuf_data(buf, &data, &len);
					err = netconn_write(newconn, data, len, NETCONN_COPY);
					#if 0
					if (err != ERR_OK) {
						printf("tcpecho: netconn_write: error \"%s\"\n", lwip_strerr(err));
					}
					#endif
					} while (netbuf_next(buf) >= 0);
				netbuf_delete(buf);
				Rx_msg = (u8_t*)data;
				for(i=0;i<len;i++){
					printf("%c",Rx_msg[i]);
				}

				struct AES_ctx ctx;
				AES_init_ctx(&ctx, key);

				for (i = 0; i < 1; ++i)
				{
					AES_ECB_decrypt(&ctx, Rx_msg + (i*16));
				}

				for(i=0;i<1;i++){
					printf("%c",Rx_msg + (i*16));
				}

				printf("\n");

				if(Rx_msg[0] == 'H' ){
					Tx_msg = "Hola clientexxxx";
				}else if(Rx_msg[0] == 'D' ){
					Tx_msg = "Es juevevesxxxxx";
				}else if(Rx_msg[0] == 'Q' ){
					Tx_msg = "Pues ahoritaxxxx";
				}else if(Rx_msg[0] == 'M' ){
					Tx_msg = "No asquisimoxxxx";
				}else{
					Tx_msg = "MensajeIncorrect";
				}
				printf("\n");

				for(i=0;i<len;i++){
					printf("%c",Tx_msg[i]);
				}

				printf("\n");

				struct AES_ctx ctx1;
				AES_init_ctx(&ctx1, key);

				for (i = 0; i < 1; ++i){
					AES_ECB_encrypt(&ctx1, Tx_msg + (i));
				}
				for(i=0;i<len;i++){
					printf("%c",Tx_msg[i]);
				}

			}
/*printf("Got EOF, looping\n");*/
/* Close connection and discard connection identifier. */
			struct AES_ctx ctx2;
			AES_init_ctx(&ctx2, key);

			for (i = 0; i < 1; ++i){
				AES_ECB_encrypt(&ctx2, Rx_msg + (i));
			}
			printf("\n");
			printf("Encriptado: ");
			printf("\n");
			for(i=0;i<len;i++){
				printf("%c",Rx_msg[i]);
			}

			netconn_write(conn, (void*)Tx_msg, (sizeof(Tx_msg)/sizeof(Tx_msg[0])), NETCONN_COPY);

			while (netbuf_next(buf) >= 0);
			netbuf_delete(buf);
			Rx_msg = (u8_t*)data;
			for(i=0;i<len;i++){
				printf("%c",Rx_msg[i]);
			}


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
