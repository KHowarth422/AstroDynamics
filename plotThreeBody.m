sun1 = readtable('Sun_1.txt');
sun2 = readtable('Sun_2.txt');
planet = readtable('Planet.txt');
planet2 = readtable('Planet_2.txt');

%% Separate Plots
subplot(1,3,1)
plot(sun1{:,'Pos_X'},sun1{:,'Pos_Y'})
grid on
title('Position of Sun_1')
xlabel('X Position (m)')
ylabel('Y Position (m)')

subplot(1,3,2)
plot(sun2{:,'Pos_X'}, sun2{:,'Pos_Y'})
grid on
title('Position of Sun_2')
xlabel('X Position (m)')
ylabel('Y Position (m)')

subplot(1,3,3)
plot(planet{:,'Pos_X'}, planet{:,'Pos_Y'})
grid on
title('Position of Planet')
xlabel('X Position (m)')
ylabel('Y Position (m)')

%% All on one plot
plot(sun1{1:1000000,'Pos_X'},sun1{1:1000000,'Pos_Y'},sun2{1:1000000,'Pos_X'},sun2{1:1000000,'Pos_Y'},planet{1:1000000,'Pos_X'},planet{1:1000000,'Pos_Y'},planet2{1:500000,'Pos_X'},planet2{1:500000,'Pos_Y'})
title('Three Body Problem')
xlabel('X Position (m)')
ylabel('Y Position (m)')
legend('Sun #1','Sun #2','Earth','Planet_2')
grid on