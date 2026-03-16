Q = eye(6)*100;
R = 1;

K_opt = dlqr(A1,B1,Q,R);
         