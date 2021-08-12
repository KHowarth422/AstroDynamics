earth = readtable('earth2.txt');
sun = readtable('sun2.txt');
subplot(1,2,1)
plot(earth{:,'Pos_X'},earth{:,'Pos_Y'})
grid on
title('Position of Earth')
xlabel('X Position (m)')
ylabel('Y Position (m)')

subplot(1,2,2)
plot(sun{:,'Pos_X'}, sun{:,'Pos_Y'})
grid on
title('Position of Sun')
xlabel('X Position (m)')
ylabel('Y Position (m)')