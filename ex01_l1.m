% atividade 1
clear; close all; clc

% a)
A = 1;
w = 5;

% sinal de 10s, quantas amostras?
% t          = 10s
% frequencia = amostras/s
% amostras   = t*(amostras/s)
t = [0:0.01:10];

s = A*cos(w*t);

figure; plot(t, s);

% b)
A_0 = 10
C_1 = 5
theta = 5

s = A_0 + C_1*cos(w*t+theta);

figure; plot(t, s);

% c)
A_0 = 10
A_1 = 2
A_2 = 2

s = A_0 + A_1*cos(w*t) + A_2*sin(w*t);

figure; plot(t, s);
