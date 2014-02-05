K_cr = 6.06;
w_n = 2.99;
P_cr = 2*pi/w_n;
Kp = 0.6*K_cr;
Kd = Kp*0.125*P_cr;
Ki = Kp*2/P_cr;

% Kp = 2.3580;
% Ki = 5.7344;
% Kd = 0.2424;
% Kp = 2.0580;
% Ki = 2.9344;
% Kd = 0.3424;

% german
% Kp = 2.13;
% Kd = 0.3328;
% Ki = 2.1289;

% Kp = 1.16;
% Kd = 0.34;
% Ki = 1.49;

% Kp = 0.97;
% Kd = 0.4;
% Ki = 0.39;

% num = [0.842 13.89];
% den = [1 5.923 18.32];
num = [16.06 43.72];
den = [1 14.37 46.82 61.35];
Ts = 0.1;

int_max = 1000;
int_min = -1000;

sim('step_test2.mdl')
Ts = 0.1;
ts = 13;
hold on
plot(ScopeData.time, ScopeData.signals.values,'b','lineWidth',2)
grid on
title('Step input response')
xlabel('time [sec]')
ylabel('Amplitude')
set(findall(gcf,'type','text'),'fontSize',ts)
set(gca,'FontSize',ts)