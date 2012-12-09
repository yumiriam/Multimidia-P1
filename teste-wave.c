#include "wavefile.h"

int main (int argc, char *argv[]) {
	HeaderType header;
	WaveData   data, data_enc, data_dec;
	short int a[6], t, b[8], i;
	int        tam;

	if (argc == 2) {
		readWave(argv[1], &header, &data);
				
		printHeader(header);
		
	//	adicionarRuido(header, data, &data_enc);
	//	writeWave("saida.wav", &header, &data);

	/*	printf("No de amostras p/ media: ");
		scanf("%d", &n);*/
	//	minimizarRuido(5, header, data);
	
	/*	decimar(2, header, data);
		decimar(4, header, data);
		decimar(8, header, data);
		decimar(16, header, data);
		decimar(32, header, data);
		decimar(64, header, data);*/
		
	//	alterarFrequencia(0.5, header, data);
	//	alterarFrequencia(2, header, data);
	//	encode_dpcm(header, data, &data_enc);
	//	decode_dpcm(header, data_enc, &data_dec);

	//	aplicarDCT(header, data, &data_enc);
	//	aplicarIDCT(header, data_enc, &data_dec);
	
	/*	a[0] = 1; a[1] = 1; a[2] = 1; a[3] = 2; a[4] = 1; a[5] = 1;
		memset(b, 0, sizeof(b));
		printf("tam: %d - ", (t = rle(a, b, 6)));
		for (i = 0;i < 8;i++) printf("%d ", b[i]);
		
		printf("tam: %d - ", unrle(b, a, t));
		for (i = 0;i < 6;i++) printf("%d ", a[i]);
		
		printf("\n");*/
		
	//	tam = aplicarRLE(header, data, &data_enc);
	//	printf("tamanho = %d\n", tam);
	//	aplicarUnRLE(header, data_enc, &data_dec, tam);
	//	printf("SRN = %d\n", calcularSNR(header, data, data_enc));
		
		mudarVolume(1, header, data, &data_enc);
		minimizarRuido(5, header, data_enc);
		
	} else {
		printf("Usage: wave-file filename.wav\n");
	}
	
	return 0;
}
