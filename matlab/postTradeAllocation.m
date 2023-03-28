function optimalAllocation = postTradeAllocation(managedAccounts, symbol1, symbol2, rollDate, filePath, accountName, whichAction, criteriaOption, sanityOnNetPosition, whichOrder)

% af: allocation factor for each account
nAccts = size(managedAccounts,1);
giveUpRef = managedAccounts(:,2);
clearingBroker = managedAccounts(:,3);
af = zeros(1,nAccts);

for j = 1:nAccts
    af(j) = managedAccounts{j,4};
end

switch symbol1
    case {'ty'}
        onePtValue = 1000;
        fileExtension = 'FOUR';
        %ff = 'UST';
    case {'crudeoil'}
        onePtValue = 1000;
        fileExtension = 'FOUR';
        %ff = 'CrudeOil';
    case {'mini'}
        onePtValue = 50;
        fileExtension = 'FOUR';
        %ff = 'Mini';
    case {'bund'}
        onePtValue = 1400;
        fileExtension = 'FOUR';
        %ff = 'Bund';
end

[data, time, buy, sell, price] = filledOrdersManager(symbol1, symbol2, filePath, whichAction, whichOrder);

start = 1;
len1 = size(data,1);
finish = len1;

%disp(['week of ', datestr(x2mdate(time(start))), ' ',datestr(x2mdate(time(finish)))]);

disp(['week of ', datestr(x2mdate(min(time))), ' ',datestr(x2mdate(max(time)))]);

len2 = finish-start+1;

time2 = time(start:finish);
netPosition = zeros(len2,1);
pnl = zeros(len2,1);
cumPnl = zeros(len2,1);
netPosAcct = zeros(len2, nAccts);
cumPnlAcct = zeros(len2, nAccts);
endDayOn = 0;

header = 'Transaction Type,Account,ClOrdID,Sec ID,Side,Qty,TPx,Exec Broker,Clearing Broker,Give-up Ref';
tmpStr = 'D01,052G1135,';
ExecBroker = 'MS';

allocator = cell(nAccts*len2+1,1);
allocator{1} = header;
counter = 1;
fprintf('Length is %d',len1);
for j = 1:len2    
    if j > 1
        lastPeriodPnL = netPosition(j-1)*(price(j)-price(j-1))*onePtValue;
        pnl(j) = lastPeriodPnL;
        cumPnl(j) = cumPnl(j-1) + lastPeriodPnL;
        netPosition(j) = netPosition(j-1) + buy(j) - sell(j);
    else
        lastPeriodPnL = 0;
        pnl(j) = lastPeriodPnL;
        cumPnl(j) = 0;
        netPosition(j) = buy(j) - sell(j);
    end
    
    if j>2
        pnlAcctSoFar = cumPnlAcct(j-1,:);
    else
        pnlAcctSoFar = zeros(1,nAccts);
    end
    %
    if j>1
        if buy(j-1) == 0
            side = -1;
            qty = sell(j-1);
        elseif sell(j-1) == 0
            side = 1;
            qty = buy(j-1);
        end        
        if j>2
            netPosAcct_ = netPosAcct(j-2,:);
        else
            netPosAcct_ = zeros(1, nAccts);
        end
        
        if endDayOn == 1
            optimalAllocation = allocation2;
            endDayOn = 0;
            counter = 1;
            
        else
            [optimalAllocation, parSet] = gridSearch2(cumPnl(j), netPosition(j-1), netPosAcct_, af, qty, pnlAcctSoFar, price(j-1), price(j), onePtValue, side, criteriaOption, sanityOnNetPosition);
            %disp(optimalAllocation*side)
        end
        
        if isempty(rollDate) == 0 
            if floor(datenum(x2mdate(data{j-1,1}))) <  datenum(rollDate)
                symbol = symbol1;
            else
                symbol = symbol2;
            end
        else
            symbol = symbol1;
        end
  
        if side == 1
            for iI = 1:nAccts
                if optimalAllocation(iI) > 0
                    counter = counter+1;
                    allocator{counter} = [tmpStr, data{j-1,2},',' symbol,',BUY,', num2str(optimalAllocation(iI)), ',', num2str(price(j-1), '%.8f'), ',', ExecBroker, ',', clearingBroker{iI,:}, ',', giveUpRef{iI,:}];
                end
            end
        elseif side == -1
            for iI = 1:nAccts
                if optimalAllocation(iI) > 0
                    counter = counter+1;
                    allocator{counter} = [tmpStr, data{j-1,2},',' symbol, ',SELL,', num2str(optimalAllocation(iI)), ',', num2str(price(j-1), '%.8f'), ',', ExecBroker, ',', clearingBroker{iI,:}, ',', giveUpRef{iI,:}];
                end
            end
        end
        
        if j>2
            netPosAcct(j-1,:) = netPosAcct(j-2,:) + side*optimalAllocation;
        else
            netPosAcct(j-1,:) = side*optimalAllocation;
        end
        disp(netPosAcct(j-1,:))
        lastPeriodAcctPnL = netPosAcct(j-1,:)*(price(j)-price(j-1))*onePtValue;
        %disp(['before:' num2str(cumPnlAcct(j,:))]);
        cumPnlAcct(j,:) = cumPnlAcct(j-1,:) + lastPeriodAcctPnL;       
        %disp(['before:' num2str(cumPnlAcct(j,:))]);
    end
    
    if j == len2 %|| floor(x2mdate(time2(j))) ~= floor(x2mdate(time2(j+1)))
        % last trade of the day or last trade of the week
        
        dateString = datestr(x2mdate(time(j)), 'yyyymmdd');
        
        if netPosition(j) == 0
            %(a) if last trade of the day makes the system flat then all 
            % sub-accounts are flat
            scenario = '1';
        elseif netPosition(j-1) == 0
            %(b) if prior to last trade system was flat, allocate according
            %to the allocation factor
            scenario = '2';
        else
            %(c) otherwise we work on optimizing of net position across
            % sub-accounts
            scenario = '3';
        end
        
        if buy(j) == 0
            side = -1;
            qty = sell(j);
        elseif sell(j) == 0
            side = 1;
            qty = buy(j);
        end
        fprintf('\n %d %d',qty, side);
        disp(netPosAcct(j-1,:));
        disp(scenario);
        allocation2 = lastTradeAllocation(qty, side, af, netPosAcct(j-1,:), scenario);
        disp(allocation2)
        if isempty(rollDate) == 0 
            if floor(datenum(x2mdate(data{j,1}))) <  datenum(rollDate)
                symbol = symbol1;
            else
                symbol = symbol2;
            end
        else
            symbol = symbol1;
        end
        
        
        if side == 1
            for iI = 1:nAccts
                if allocation2(iI) > 0
                    counter = counter+1;
                    allocator{counter} = [tmpStr, data{j,2},',' symbol,',BUY,',num2str(allocation2(iI)), ',', num2str(price(j), '%.8f'), ',', ExecBroker, ',', clearingBroker{iI,:}, ',', giveUpRef{iI,:}];
                end
            end
        elseif side == -1
            for iI = 1:nAccts
                if allocation2(iI) > 0
                    counter = counter+1;
                    allocator{counter} = [tmpStr, data{j,2},',' symbol, ',SELL,',num2str(allocation2(iI)), ',', num2str(price(j), '%.8f'), ',', ExecBroker, ',', clearingBroker{iI,:}, ',', giveUpRef{iI,:}];
                end
            end
        end
        endDayOn = 1;
        
        fileToWrite = [lastPeriodPnL cumPnl(len2) netPosition(len2) (netPosAcct(len2-1,:)+side*allocation2) cumPnlAcct(len2,:) price(len2)];
        fileName2 = ['cumPnl_', symbol1,num2str(af(1)), '.csv' ];
        dlmwrite(fileName2, fileToWrite, 'delimiter', ',', 'precision', '%.4f');
        
        time0 = floor(time);
        [time0, m, ~] = unique(time0,'last');
        cumPnlAcct0 = cumPnlAcct(m,:);
        
        output = [0, af; time0, cumPnlAcct0];
        I = isnan(output);
        output(I) = 0;
        I = isinf(output);
        output(I) = 0;
        
        %dlmwrite(['\postTradeAllocation2\assetAllocation\doc\' symbol1, '_pnl_FOUR',num2str(af(1)),'.csv'],output,'precision', '%.6f');
        dlmwrite(['D:\Google Drive\Research&Presentations\postTradeAllocation\' symbol1, '_pnl_FOUR',num2str(af(1)),'.csv'],output,'precision', '%.6f');
        if floor(x2mdate(time2(j))) ~= floor(x2mdate(time2(start)))
            flag_0 = 1;
        else
            flag_0 = 0;
        end
        
        filename = [accountName, '_', fileExtension, '_', dateString, '_', whichAction,'.txt'];
        if exist(filename, 'file') == 0
            fid = fopen(filename, 'wt');
            flag = 0;
        else
            fid = fopen(filename, 'at');
            flag = 1;
        end
        
        if flag == 0
            fprintf(fid, '%s\n', allocator{1});
        end

        if flag_0 == 0
            for i = 2:counter
                fprintf(fid, '%s\n', allocator{i}); 
            end
        elseif flag_0 == 1
            for i = 2:counter
            %for i = 3:counter
                fprintf(fid, '%s\n', allocator{i}); 
            end
        end
        
        fclose(fid);
        
        netPosAcct(len2,:) = netPosAcct(len2-1,:)+side*allocation2;
        
        disp(' ');
        disp('=======================================');
        
        disp('last time and minMae');
        disp([datestr(x2mdate(time2(j))), ' ', num2str(parSet.minMae)]);
        disp('cum p&l (scaled)');
        disp([num2str(cumPnl(j)), ' ', num2str(cumPnlAcct(j,:)./af)]);
        disp('cum p&l (unscaled)');
        disp([num2str(cumPnl(j)), ' ', num2str(cumPnlAcct(j,:))]);
        disp('net position at len2-1');
        disp(num2str(netPosAcct(j-1,:)));
        disp('net position at the end');
        disp(num2str(netPosAcct(len2,:)));
        disp('last two prices');
        disp([num2str(price(j-1)), ' ', num2str(price(j))])
        disp('net position at the fund level');
        disp(num2str(netPosition(j)));
        disp('side multiplied qty');
        disp(num2str(side*qty));
        disp('optimal allocation');
        disp(num2str(allocation2));
        
        disp('-------------------------------------');
        
    end
    %disp(num2str(netPosAcct(j-1,:)));

end

file_id = fopen('position.txt', 'wt');
for j=1:len2
    fprintf(file_id, "%d,%d,%d,%d,%d,%d\n",netPosAcct(j,:));
end
fclose(file_id);