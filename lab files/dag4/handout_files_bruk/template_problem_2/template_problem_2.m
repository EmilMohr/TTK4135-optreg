% TTK4135 - Helicopter lab
% Hints/template for problem 2.
% Updated spring 2018, Andreas L. Flåten

%% Initialization and model definition
init05; % Change this to the init file corresponding to your helicopter
dag4fuck;
% Discrete time system model. x = [lambda r p p_dot]'
delta_t	= 0.25; % sampling time
A1 = [1 delta_t 0 0 0 0;
      0 1 -delta_t*K_2 0 0 0;
      0 0 1 delta_t 0 0;
      0 0 -delta_t*K_1*K_pp 1-delta_t*K_1*K_pd 0 0;
      0 0 0 0 1 delta_t;
      0 0 0 0 -delta_t*K_3*K_ep 1-delta_t*K_3*K_ed];
B1 = [0 0;
      0 0;
      0 0;
      delta_t*K_1*K_pp 0;
      0 0;
      0 delta_t*K_3*K_ep];

% Number of states and inputs
mx = size(A1,2); % Number of states (number of columns in A)
mu = size(B1,2); % Number of inputs(number of columns in B)

% Initial values
x1_0 = pi;                               % Lambda
x2_0 = 0;                               % r
x3_0 = 0;                               % p
x4_0 = 0;                               % p_dot
x0 = [x1_0 x2_0 x3_0 x4_0 0 0]';           % Initial values

% Time horizon and initialization
N  = 40;                                  % Time horizon for states
M  = N;                                 % Time horizon for inputs
z  = zeros(N*mx+M*mu,1);                % Initialize z for the whole horizon
z0 = z;                                 % Initial value for optimization

% Bounds
ul 	    = -pi/6;                   % Lower bound on control
uu 	    = pi/6;                   % Upper bound on control

xl      = -Inf*ones(mx,1);              % Lower bound on states (no bound)
xu      = Inf*ones(mx,1);               % Upper bound on states (no bound)
xl(3)   = ul;                           % Lower bound on state x3
xu(3)   = uu;                           % Upper bound on state x3

% Generate constraints on measurements and inputs
[vlb,vub]       = gen_constraints(N,M,xl,xu,ul, uu); % hint: gen_constraints
vlb(N*mx+M*mu)  = 0;                    % We want the last input to be zero
vub(N*mx+M*mu)  = 0;                    % We want the last input to be zero

% Generate the matrix Q and the vector c (objecitve function weights in the QP problem) 
Q1 = zeros(mx,mx);
Q1(1,1) =1 ;                            % Weight on state x1
%Q1(2,2) = ;                            % Weight on state x2
Q1(3,3) = 1;                            % Weight on state x3
%Q1(4,4) = ;                            % Weight on state x4
P1 = 1.2;                                % Weight on input
Q = gen_q(Q1, P1,N,M);                                 % Generate Q, hint: gen_q
c = zeros((N*mx+M*mu),1);                                  % Generate c, this is the linear constant term in the QP

%% Generate system matrixes for linear model
Aeq = gen_aeq(A1, B1, N, mx, mu);             % Generate A, hint: gen_aeq
beq = zeros(N*mx,1);
beq(1:mx)= A1*x0;% Generate b

%% Solve SQP
q1 = 1;
q2 = 1;

costfun = @(z) objectfunc(z,N,mx,mu,0,q1,q2);
nonlcon = @(z) nonlinearcon(z,N,mx);
%% Solve QP problem with linear model
tic
%[z,lambda] = quadprog(Q,c, [], [], Aeq,beq, vlb, vub); % hint: quadprog. Type 'doc quadprog' for more info 
[z, lambda] = fmincon(costfun,z0,[],[],Aeq,beq,vlb,vub,nonlcon);
t1=toc;

% Calculate objective value
%phi1 = 0.0;
%PhiOut = zeros(N*mx+M*mu,1);
%for i=1:N*mx+M*mu
%  phi1=phi1+Q(i,i)*z(i)*z(i);
%  PhiOut(i) = phi1;
%end

%% Extract control inputs and states
%u  = [z(N*mx+1:N*mx+M*mu);z(N*mx+M*mu)]; % Control input from solution

x1 = [x0(1);z(1:mx:N*mx)];              % State x1 from solution travel
x2 = [x0(2);z(2:mx:N*mx)];              % State x2 from solution
x3 = [x0(3);z(3:mx:N*mx)];              % State x3 from solution
x4 = [x0(4);z(4:mx:N*mx)];              % State x4 from solution
x5 = [x0(5);z(5:mx:N*mx)];              %elevation
x6 = [x0(6);z(6:mx:N*mx)];              %elevation rate
u1 = [0;z(N*mx+1:mu:N*(mx+mu))];
u2 = [0;z(N*mx+2:mu:N*(mx+mu))];

num_variables = 10/delta_t;
zero_padding = zeros(num_variables,1);
unit_padding  = ones(num_variables,1);

u1   = [zero_padding; u1; zero_padding];
u2   = [zero_padding; u2; zero_padding];
x1  = [pi*unit_padding; x1; zero_padding];
x2  = [zero_padding; x2; zero_padding];
x3  = [zero_padding; x3; zero_padding];
x4  = [zero_padding; x4; zero_padding];
x5  = [zero_padding; x5; zero_padding];
x6  = [zero_padding; x6; zero_padding];
uT = u1';
t = 0:delta_t:delta_t*(length(u1)-1);
M = [t; uT];
Mn = [t;u2'];

%% Plotting
M1 = [t; x1'];
M2 = [t; x2'];
M3 = [t; x3'];
M4 = [t; x4'];
M5 = [t; x5'];
M6 = [t; x6'];

save("optimalbanepitch", "M")
save("optimalbaneelev", "Mn")
save("x1", "M1")
save("x2", "M2")
save("x3", "M3")
save("x4", "M4")
save("x5", "M4")
save("x6", "M4")
% save("q_0,12_oppgaag2pdot", "x4")
% save("q_0,12_ove3dag2lambda", "x1")
% save("q_0,12_ppgave3dg2r", "x2")

save("u_vector1","M")
figure(2)
subplot(511)
stairs(t,u1),grid
ylabel('u')
subplot(512)
plot(t,x1,'m',t,x1,'mo'),grid
ylabel('lambda')
subplot(513)
plot(t,x2,'m',t,x2','mo'),grid
ylabel('r')
subplot(514)
plot(t,x3,'m',t,x3,'mo'),grid
ylabel('p')
subplot(515)
plot(t,x4,'m',t,x4','mo'),grid
xlabel('tid (s)'),ylabel('pdot')
subplot(516)
plot(t,x5,'m',t,x5','mo'),grid
xlabel('tid (s)'),ylabel('elev')
ylabel('e')
subplot(517)
plot(t,x6,'m',t,x6','mo'),grid
xlabel('tid (s)'),ylabel('elev_dot')
ylabel('e_dot')