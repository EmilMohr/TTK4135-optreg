Q = eye(4)*0.01;
R = 1;

K_opt = dlqr(A1,B1,Q,R);
         