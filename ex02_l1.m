% atividade 2
clear; close all; clc

% a)
t = [0:0.01:10];

s = sin(t);

figure; plot(t, s);

% b)
s = sin(t) + (1/3)*sin(3*t);

figure; plot(t, s);

% c)
s = sin(t) + (1/3)*sin(3*t) + (1/5)*sin(5*t);

figure; plot(t, s);
