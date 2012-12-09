#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define RND_MAX 1023
#define PI 3.14
#define E 2.72

/* int          4bytes */
/* short int    2bytes */

typedef struct {
	int ChunkID;                    /* 46464952 (RIFF) */
	int ChunkSize;                  /*  */
	int Format;                     /* WAVE-0X57415645 */
	
	int Subchunk1ID;                /* fmt */
	int Subchunk1Size;            
	unsigned short int AudioFormat; /* PCM = 1 */
	unsigned short int NumChannels; /* 1 = Mono, 2 = Stereo */
	int SampleRate;                 /* Frequência */
	int ByteRate;                   /* SR * NC * BitsPerSample/8 */
	unsigned short int BlockAlign;  /* NC * BitsPerSample/8 */
	unsigned short int BitsPerSample;
	
	int Subchunk2ID;                /* 61746164 (DATA) */
	int Subchunk2Size;              /* Bytes in the data */
} HeaderType;

typedef struct {
	short int    *Sample;
} WaveData;

/* DCT ex30a_l1 */
void dct(short int *sample, short int *result, int n);
/* DCT ex30c_l1 */
void idct(short int *sample, short int *result, int n);

/* RLE ex31_l1 */
int rle(short int *sample, short int *result, int n);
/* RLE ex31_l1 */
int unrle(short int *sample, short int *result, int n);

/* SNR ex34_l1 */
int snr(short int *s, short int *o, int n);

/* Imprimir cabecalho de arquivo WAVE lido ex06_l1 */
void printHeader(HeaderType header);

/* Escrever arquivo WAVE */
void writeWave(char *filename, HeaderType *header, WaveData *data);
/* Ler arquivo WAVE filename.wav                     /
/  Retorna o cabecalho em header e os dados em data */
void readWave(char *filename, HeaderType *header, WaveData *data);

/* Adicionar ruido ex17a_l1 */
void adicionarRuido(HeaderType header, WaveData data, WaveData *ruido);
/* Minimizar ruido ex17b_l1 */
void minimizarRuido(int n, HeaderType header, WaveData data);
/* Minimizar ruido usando dominio de Fourier ex17c_l1 */
void minRuidoFourier(HeaderType header, WaveData data);

/* Alterar frequencia de amostragem ex13_l1 */
void alterarFrequencia(float fat, HeaderType header, WaveData data);
/* Decimacao ex14_l1 */
void decimar(int fat, HeaderType header, WaveData data);

/* DPCM ex29_l1 */
void encode_dpcm(HeaderType header, WaveData data, WaveData *enc_data);
/* DPCM ex29_l1 */
void decode_dpcm(HeaderType header, WaveData data, WaveData *dec_data);

/* Aplicar a DCT no arquivo WAVE ex30a_l1 */
void aplicarDCT(HeaderType header, WaveData data, WaveData *dct_data);
/* Aplicar a IDCT no arquivo WAVE ex30c_l1 */
void aplicarIDCT(HeaderType header, WaveData dct_data, WaveData *idct_data);
/* Aplicar o RLE no arquivo WAVE ex31_l1 */
int  aplicarRLE(HeaderType header, WaveData data, WaveData *rle_data);
/* Recuperar do RLE no arquivo WAVE ex31_l1 */
int  aplicarUnRLE(HeaderType header, WaveData data, WaveData *rle_data, int tam);

/* SNR no arquivo WAVE ex31_l1 */
int  calcularSNR(HeaderType header, WaveData data, WaveData ruido_data);

/* Variar a amplitude (volume) ex36_l1 */
void mudarVolume(int fat, HeaderType header, WaveData data, WaveData *vol_data);
