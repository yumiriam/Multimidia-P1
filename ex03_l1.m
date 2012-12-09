% atividade 3
clear; close all; clc

% a)
t = [0:0.1:1000];

s = sin(t);

figure; plot(t, s);

fft_s = fft(s);
fft_real_s = real(fft_s);

figure; plot(fft_real_s);

% b)
s = sin(t) + (1/3)*sin(3*t);

figure; plot(t, s);

fft_s = fft(s);
fft_real_s = real(fft_s);

figure; plot(fft_real_s);

% c)
s = sin(t) + (1/3)*sin(3*t) + (1/5)*sin(5*t);

figure; plot(t, s);

fft_s = fft(s);
fft_real_s = real(fft_s);

figure; plot(fft_real_s);
