#include <stdio.h>
#include <string.h>
#include "pbc.h"

/*
[0x23, 0x3A, 0x46, 0x4C, 0x52] ==> “233A464C52”
const uint8_t * src_buf: input, point to the source
int src_len: input, indicate the source length, should greater than 0
uint8_t * dest_buf: output, point to the destination
int dest_len: input, indicate the destination length, 
        should be greater or equal than src_len * 2
*/
uint32_t ByteStrToHexStr(const uint8_t * src_buf, int src_len, 
    uint8_t * dest_buf, int dest_len)
{
    if(NULL == src_buf || NULL == dest_buf ||
         src_len <= 0 || dest_len < src_len * 2)
	{
        printf("ByteStrToHexStr input error\n");
        return -1;
    }	
    uint8_t highHex, lowHex;
    const uint8_t * index = src_buf, * end = src_buf + src_len;
    uint8_t * ridx = dest_buf;
    
    while (index < end)
    {
        highHex = (* index) >> 4;
        lowHex = (* index) & 0x0F;
        index ++;

        if (highHex > 0x09)
            highHex += 0x57;
        else
            highHex += 0x30;

        if (lowHex > 0x09)
            lowHex += 0x57;
        else
            lowHex += 0x30;

        *ridx ++ = highHex;
        *ridx ++ = lowHex;
    }
    return 0;
}

/*
 “233A464C52” ==>[0x23, 0x3A, 0x46, 0x4C, 0x52]
const uint8_t * src_buf: input, point to the source
int src_len: input, indicate the source length, should greater than 0, should be devided by 2
uint8_t * dest_buf: output, point to the destination
int dest_len: input, indicate the destination length, 
        should be greater or equal than src_len / 2
*/
uint32_t HexStrToByteStr(const uint8_t * src_buf, int src_len, 
    uint8_t * dest_buf, int dest_len)
{
    if(NULL == src_buf || NULL == dest_buf ||
         src_len <= 0 || 
         (src_len % 2 != 0) ||
         (dest_len < src_len / 2)
         )
	{
        printf("HexStrToByteStr input error\n");
        return -1;
    }	
    uint8_t highByte, lowByte;
	const uint8_t * index = src_buf, * end = src_buf + src_len;
    uint8_t * ridx = dest_buf;
    
    while (index < end)
    {
        highByte = tolower(* (index ++));
        lowByte  = tolower(* (index ++));

        if (highByte > 0x39)
            highByte -= 0x57;
        else
            highByte -= 0x30;

        if (lowByte > 0x39)
            lowByte -= 0x57;
        else
            lowByte -= 0x30;

        *ridx ++ = (highByte << 4) | lowByte;
    }
    return 0;
}

int main(int argc, char *argv[])
{

    pairing_t pairing;
    element_t g;
    element_t Z;
    char param_str[] = "type a\n"
                       "q 8780710799663312522437781984754049815806883199414208211028653399266475630880222957078625179422662221423155858769582317459277713367317481324925129998224791\n"
                       "h 12016012264891146079388821366740534204802954401251311822919615131047207289359704531102844802183906537786776\n"
                       "r 730750818665451621361119245571504901405976559617\n"
                       "exp2 159\n"
                       "exp1 107\n"
                       "sign1 1\n"
                       "sign0 1";
    pbc_param_t par;
    pbc_param_init_set_str(par, param_str);
    pairing_init_pbc_param(pairing, par);

    element_init_G1(g, pairing);

    element_random(g);

    element_printf("g = %B\n", g);

    int g_len =  element_length_in_bytes(g);
    
    uint8_t * g_bytes = (uint8_t *) malloc(g_len);

    element_to_bytes(g_bytes, g);

    printf("export g, pk_len = %d, pk_bytes=\n", g_len);
    for(int i=0;i<g_len;i++){
        printf("%02x ", g_bytes[i]);
    }
    printf("\n");

    uint8_t  g_b2[] = {
        0x91,0xce,0x6a,0x92,0xd1,0xef,0x78,0x98,0x11,0xea,0x83,0x0e,0x72,0xc4,0x64,0x05,0xdf,0x65,0x52,0x53,0x98,0x21,0x23,0x4d,0xba,0x7a,0xb0,0x06,0x99,0x44,0x46,0x86,0x99,0xaf,0x56,0x53,0xd0,0x00,0x3b,0x23,0xf5,0x0b,0x48,0x0b,0xfc,0x11,0x0d,0xdd,0x50,0x44,0xd8,0x71,0x17,0xe5,0xff,0x48,0x02,0xc2,0xde,0x97,0x79,0x63,0x26,0x47,0x5f,0xe0,0x18,0x89,0x60,0x27,0x5c,0x36,0x0b,0x0a,0xce,0x33,0x7a,0xf7,0x61,0x98,0xd6,0xfc,0x3c,0xe5,0x3b,0x95,0x84,0xde,0x17,0x6f,0x0f,0x00,0xf2,0x40,0x04,0x5d,0xb7,0x6c,0x0d,0x5a,0xa5,0x0f,0x3e,0x31,0xef,0x20,0xbb,0x36,0xc3,0x5d,0x9e,0xae,0xb2,0xd5,0x65,0x3d,0x96,0xd4,0x47,0xed,0x32,0x5e,0x51,0x09,0x99,0x1d,0xd5,0xc6
    };
    element_t gg;
    element_init_G1(gg, pairing);
    element_from_bytes(gg, (uint8_t *)g_b2);
    element_printf("gg = %B\n", gg);
    
    uint8_t *hex = "91ce6a92d1ef789811ea830e72c46405df6552539821234dba7ab0069944468699af5653d0003b23f50b480bfc110ddd5044d87117e5ff4802c2de97796326475fe0188960275c360b0ace337af76198d6fc3ce53b9584de176f0f00f240045db76c0d5aa50f3e31ef20bb36c35d9eaeb2d5653d96d447ed325e5109991dd5c6";
    uint8_t  g_b3[256];
    int hex_len = strlen(hex);
    HexStrToByteStr(hex, hex_len, g_b3, 256);
    element_t g3;
    element_init_G1(g3, pairing);
    element_from_bytes(g3, (uint8_t *)g_b3);
    element_printf("g3 = %B\n", g3);
    return 0;
}