% exercicio 37
close all; clear all; clc
% a)

%  sinetone (freq, rate, sec, ampl)
%    freq = frequency
%    sec  = length (seconds)
%    rate = sampling rate
%    ampl = amplitude
%  freq and ampl may be vectors of common size.
%  defaults are rate = 8000, sec = 1 and ampl = 64.

freq = 435;
rate = 8000;
sec  = 5;
ampl = 16;

%data = sinetone(freq, rate, sec, ampl);
%wavwrite(data, "ex37a_l1.wav");

bps   = 16;       % bits per sample
sps   = 8000;     % sample rate [samples/s]
freq  = 435;      % frequency of the tone [Hz]
nsecs = 5;        % number of seconds of the audio file

nsamples = sps*nsecs;

time = linspace(0, nsecs, nsamples);
wave = sin(time*2*pi*freq)';

wavwrite(wave, sps, bps, 'ex37a_l1.wav');
figure, plot(wave);

harm = zeros(size(wave));
for i = 1:5
	harm = harm + (sin(time*i*2*pi*freq)' / i);
end

wavwrite(harm, "ex37b_l1.wav");
figure, plot(harm);

harm = zeros(size(wave));
for i = 1:10
	harm = harm + (sin(time*i*2*pi*freq)' / i);
end

wavwrite(harm, "ex37c_l1.wav");
figure, plot(harm);

% d)
f = 435;

# intervalos com relacao a f de do
%             do     re      mi       fa     sol     la     si
intervalos = [    1,  (9/8),   (5/4), (4/5), (3/2),  (5/3),  (15/8)];
%              (9/8), (10/9), (16/15), (9/8), (10/9), (9/8), (16/15)];

% tom maior =   (9/8)
% tom menor =  (10/9)
% semi-tom  = (16/15)
% oitava    =     (2)
   
sustenido  = (25/24)*f;
bemol      = (24/25)*f;

do3  = 435*(1 / intervalos(1, 4)); % fa3 x I
%sol3 = do3*intervalo(1, 5);
do4  = do3*2;
%re4  = do4*intervalo(1, 2);
%mi4  = do4*intervalo(1, 3);
%fa4  = do4*intervalo(1, 4);
%sol4 = do4*intervalo(1, 5);

notas3 = do3*intervalos;
notas4 = do4*intervalos;

nsecs = 1;
nsamples = sps*nsecs;

time = linspace(0, nsecs, nsamples);

[n, m] = size(sin(time*2*pi*notas3(1, 1)));

sinais3 = zeros(7, m);
sinais4 = zeros(7, m);

for i = 1:7
	sinais3(i, :) = sin(time*2*pi*notas3(1, i));
	sinais4(i, :) = sin(time*2*pi*notas4(1, i));
end

% Sol3-Dó4-Dó4-Mí4-Mí4-Sol4-Sol4
% Fá4-Mí4-Ré4-Sol4-Mí4-Ré4-Dó4
musica = [sinais3(5, :), sinais4(1, :), sinais4(1, :), sinais4(3, :), sinais4(3, :), sinais4(5, :), sinais4(5, :), sinais4(4, :), sinais4(3, :), sinais4(2, :), sinais4(5, :), sinais4(3, :), sinais4(2, :), sinais4(1, :)]';

wavwrite(musica, "ex37d_l1.wav");
figure, plot(musica);
