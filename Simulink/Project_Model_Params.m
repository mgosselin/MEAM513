K_cr = 3.55;
w_n = 3.14;
P_cr = 2*pi/w_n;
Kp = 0.6*K_cr;
Kd = Kp*0.125*P_cr;
Ki = Kp*2/P_cr;

% Kp = 3;
% Kd = 0.5;
% Ki = 2;
% Kp = 2.0580;
% Ki = 2.9344;
% Kd = 0.9424;

d_cm = 10;
lead_v = 30;

FF = 30;
int_max = 150;
int_min = -150;

init_d = 12;

duty_max = 100;
duty_min = 0;

offset = -20;

Ts = 0.1;

num = [15.12];
den = [1 5.18 14.12];

sim('Project_Model_2.mdl');