% num = [15.12];
% den = [1 5.18 14.12];

num = [0.842 13.89];
den = [1 5.923 18.32];


% K_cr = 3.55;
% w_n = 3.14;
% P_cr = 2*pi/w_n;
% Kp = 0.6*K_cr;
% Kd = Kp*0.125*P_cr;
% Ki = Kp*2/P_cr;

K_cr = 6.23;
w_n = 3.85;
P_cr = 2*pi/w_n;
Kp = 0.6*K_cr;
Kd = Kp*0.125*P_cr;
Ki = Kp*2/P_cr;

% Kp = 1.858;
% Ki = 2.7344;
% Kd = 0.2424;

% gc = tf(1,1);
% gp = tf(num,den);
% gi = tf(1,[s 0]);
% set(g0,'inputdelay',0.1);
% gpade = pade(g0,1);

% set(gp,'inputdelay',0.1);
% gpade = pade(gp,1);

% num = [-15.12 302.4 0];
% den = [1 25.18 118.72 302.4 0 0];

% mike kd
% num = [-15.12 302.4 0 0];
% den = [1 25.18 118.72 284.86 350.78 0];

% chao kd
% num = [-15.12 302.4 0 0];
% den = [1 25.18 118.72 287.74 393.3 0];

% mike ki
% num = [-0.0421 0.1475 13.89];
% den = [0.05 1.2961 6.839 18.32 0];


num = [-0.803 13.874 43.72];
den = [0.05 1.7185 16.71 49.88 61.35 0];

sys = tf(num,den);

% sys = gc*gpade;
k = 0:0.01:100;
rlocus(sys,k);
% sim('step_test.mdl')
% Ts = 0.1;
% ts = 13;
% hold on
% plot(ScopeData.time, ScopeData.signals.values,'lineWidth',2)
% grid on
% title('Step input response')
% xlabel('time [sec]')
% ylabel('Amplitude')
% set(findall(gcf,'type','text'),'fontSize',ts)
% set(gca,'FontSize',ts)