%fileID = fopen('PointsToPlot.txt','r');
%formatSpec = '%d %f';
%sizeA = [Inf 4];
%A = fscanf(fileID,formatSpec,sizeA);

%mat = [3 5 3 1; 3 5 10 3; 3 5 2 7; 2 7 5 9; 5 9 6 8];
%plot(mat(:,[1,3])', mat(:,[2,4])', 'o-')


%figure(1);
%A = dlmread('./Code/Outputs/0.txt');
%plot(A(:,[1,3])', A(:,[2,4])', 'o-');

%figure(2);
%A = dlmread('./Code/Outputs/1.txt');
%plot(A(:,[1,3])', A(:,[2,4])', 'o-');

%% This goes through all the files and displays the graph at each point
prompt = 'How many Steiner Points were added? ';
x = input(prompt);
Original = dlmread('./Code/Outputs/0.txt');
plot(Original(:,[1,3])', Original(:,[2,4])', 'og-');
pause(6);
%figure(2);
for a = 0:x
    A = dlmread(sprintf('./Code/Outputs/%i.txt', a));
    plot(A(:,[1,3])', A(:,[2,4])', 'om-');
    hold on;
    plot(A(end,[1,3])', A(end,[2,4])', 'om-');
    pause(1);
    hold off;
end