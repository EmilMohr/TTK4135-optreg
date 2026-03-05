function J = objectfunc(z,N,mx,mu,lambda_f,q1,q2)
J = 0;
offset = N*mx;
for k = 1:N
    lambda_k = z((k-1)*mx+1);
    J = J + (lambda_k - lambda_f)^2;
end
for k= 0:(N-1)
    uk = z(offset + k*mu+(1:mu));
    pc=uk(1);
    ec=uk(2);
    J = J+q1*pc^2+q2*ec^2;
end
end
