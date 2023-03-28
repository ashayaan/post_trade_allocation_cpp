function output = readFills(filePath, market)

% month1 = dateStr(1:2);
% year1 = dateStr(5:8);
%fileName = ['\postTradeAllocation2\assetAllocation\executions\' market '.csv'];
disp(market);
%fileName = 'D:\Google Drive\Research&Presentations\postTradeAllocation\filledOrders.csv';
%fileName = ['D:\Google Drive\Research&Presentations\postTradeAllocation\orders_', market];
fileName = 'orders_ty.csv';
fid = fopen(fileName);
data = textscan(fid,'%s','delimiter', ',');
fclose(fid);


data = data{1};
N = length(data);
disp(N);
nLine = N/7-1;
data = reshape(data,7,N/7);

%nLine = N/6-1;
%data = reshape(data,6,N/6);

data = data';
data = regexprep(data, '"', '');
data = regexprep(data, ':', ' ');

output = cell(nLine,6);

for i = 1:nLine
%     tick = str2num(data{i+1,2});  %#ok<ST2NM>
%     tickTime = tick(1)/24+ tick(2)/24/60 + tick(3)/24/60/60 + tick(4)/24/60/60/1000;
%     
%     output{i,1} = m2xdate(datenum(data{i+1,1})+ tickTime);


    % execution date/time
    output{i,1} = str2double(data{i+1,2});
    
    % symbol
    output{i,2} = data{i+1,3};
     
    %order id
    output{i,3} = data{i+1,4};
    
    % price
    output{i,4} = str2double(data{i+1,5});
    
    % buy or sell
    output{i,5} = 0;
    output{i,6} = 0;
    if str2num(data{i+1,7})==0 %#ok<ST2NM>
        % side = 0 for buy
        output{i,5} = str2num(data{i+1,6}); %#ok<ST2NM>
    else
        % side = 1 for sell
        output{i,6} = str2num(data{i+1,6}); %#ok<ST2NM>
    end
end

