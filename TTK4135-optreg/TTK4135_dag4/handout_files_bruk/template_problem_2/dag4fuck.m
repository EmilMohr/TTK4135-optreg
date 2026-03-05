N = 40;
function [c, ceq] = nonlinearcon(z, N)

alpha = 0.2; beta = 20; lambda_t = 2*pi/3;
c = zeros(N,1);
for k = 1:N
    c (k) = alpha * exp(-beta *(z((k-1)*mx + 1) - lambda_t)^2) - z((k-1)*mx +5);
end

ceq = [];
end

