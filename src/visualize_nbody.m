%{
manevra(:,1) = strrep(manevra(:,1),'#','')
test = split(manevra(:,1),'/')
manevraobj0 = []
manevraobj1 = []
manevraobj2 = []
t=0
for i=1:1:length(test)
    if str2num(test(i,1)) == 1
        manevraobj1 = [manevraobj1; str2num(test(i,2))]
    end
    if str2num(test(i,1)) == 2
        manevraobj2 = [manevraobj2; str2num(test(i,2))]
    end
    if str2num(test(i,1)) == 0
        manevraobj0 = [manevraobj0; str2num(test(i,2))]
    end
end
%}
h = animatedline('Color','r','LineWidth',3,'LineStyle','-','DisplayName','UYDU1','MaximumNumPoints',55000);
%h2 = animatedline('Color','g','LineWidth',3,'LineStyle','-','DisplayName','UYDU2');
%h3 = animatedline('Color','b','LineWidth',6,'LineStyle','-','DisplayName','UYDU3','MaximumNumPoints',550);
viscircles([0,0], 6371000,'color','b');

set(gcf,'Name','Orbital Maneuever Simulation Test') %select the name you want
grid on
grid minor
axis equal

title(['N-Body Simulation'])
xlabel('X POZISYONU')
ylabel('Y POZISYONU')

xlim([min(logging(:,2)) max(logging(:,2))])
ylim([min(logging(:,3)) max(logging(:,3))])
hold on 
set(gcf,'color',[0.658, 1, 0.968]);
set(gca,'GridColor',[1 1 1]) % a bluish color
set(gca,'Color','k')
set(gca,'LineWidth',4) 
drawnow

for i=1:1:length(logging)
    if(logging(i,1) == 0)

    end
    if(logging(i,1) == 1)
        addpoints(h,logging(i,2),logging(i,3));
    end
drawnow limitrate
    pause(0.00001);

end
