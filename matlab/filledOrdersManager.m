function [dataOverwrite, time, buy, sell, price] = filledOrdersManager(symbol1, symbol2, filePath, whichAction, whichOrder)

if length(whichOrder) >= 1
    data1 = readFills(filePath, symbol1);
end

% Execution:
% date/time  symbol    orderID   price     buy       sell
% data{:,1}  data{:,2} data{:,3} data{:,4} data{:,5} data{:,6}

% %
% if length(whichOrder) >= 2
%     data2 = readFills('10262011', symbol2);
% end
% %
% if length(whichOrder) >= 3
%     data3 = readFills('10272011', symbol2);
% end
% %
% if length(whichOrder) >= 4
%     data4 = readFills('10282011', symbol2);
% end
% %
% if length(whichOrder) >= 5
%     data5 = readFills('10292011', symbol2);
% end

% if length(whichOrder) >= 1 && (whichOrder(1) == -1 || whichOrder(1) == 1)
%     len1 = size(data1, 1);
%     price = zeros(len1,1);
%     for i=1:len1
%         price(i) = data1{i,4};
%     end
%     [price1, iP] = sort(price);
%     data1(1:end,:) = data1(iP,:);
%     if whichOrder(1) == -1
%         data1(1:end,:) = data1(end:-1:1,:);
%     end
% end
% 
% if length(whichOrder) >= 2 && (whichOrder(2) == -1 || whichOrder(2) == 1)
%     len2 = size(data2, 1);
%     price = zeros(len2,1);
%     for i=1:len2
%         price(i) = data2{i,4};
%     end
%     [price1, iP] = sort(price);
%     data2(1:end,:) = data2(iP,:);
%     if whichOrder(2) == -1
%         data2(1:end,:) = data2(end:-1:1,:);
%     end
% end
% 
% if length(whichOrder) >= 3 && (whichOrder(3) == -1 || whichOrder(3) == 1)
%     len3 = size(data3, 1);
%     price = zeros(len3,1);
%     for i=1:len3
%         price(i) = data3{i,4};
%     end
%     [price1, iP] = sort(price);
%     data3(1:end,:) = data3(iP,:);
%     if whichOrder(3) == -1
%         data3(1:end,:) = data3(end:-1:1,:);
%     end
% end
% 
% if length(whichOrder) >= 4 && (whichOrder(4) == -1 || whichOrder(4) == 1)
%     len4 = size(data4, 1);
%     price = zeros(len4,1);
%     for i=1:len4
%         price(i) = data4{i,4};
%     end
%     [price1, iP] = sort(price);
%     data4(1:end,:) = data4(iP,:);
%     if whichOrder(4) == -1
%         data4(1:end,:) = data4(end:-1:1,:);
%     end
% end
% 
% 
% if length(whichOrder) >= 5 && (whichOrder(5) == -1 || whichOrder(5) == 1)
%     len5 = size(data5, 1);
%     price = zeros(len5,1);
%     for i=1:len5
%         price(i) = data5{i,4};
%     end
%     [price1, iP] = sort(price);
%     data5(1:end,:) = data5(iP,:);
%     if whichOrder(5) == -1
%         data5(1:end,:) = data5(end:-1:1,:);
%     end
% end

if length(whichOrder) == 1
    data = data1;
end

% if length(whichOrder) == 2
%     data = [data1; data2];
% end
% 
% if length(whichOrder) == 3
%     data = [data1; data2; data3];
% end
% 
% if length(whichOrder) == 4
%     data = [data1; data2; data3; data4];
% end
% 
% if length(whichOrder) == 5
%     data = [data1; data2; data3; data4; data5];
% end

% temp = cell2mat(data(:,4));
% index = temp>0;
% data = data(index,:);

switch whichAction
    
    case 'sort'
        
        len1 = length(data);
        price = zeros(len1,1);
        for i=1:len1
            price(i) = data{i,4};
        end
        [~, iP] = sort(price);
        data(1:end,:) = data(iP,:);
        if whichOrder == -1
            data(1:end,:) = data(end:-1:1,:);
        end
        
        time = zeros(len1,1);
        buy = zeros(len1,1);
        sell = zeros(len1,1);
        price = zeros(len1,1);
        
        for j = 1:len1
            time(j)  = data{j,1};
            buy(j)  = data{j,5};
            sell(j) = data{j,6};
            price(j) = data{j,4};
        end
        
        dataOverwrite = data;
    
    case 'contract'
        
        len1 = length(data);
        
        dataOverwrite = cell(len1,6);

        time = zeros(len1,1);
        buy = zeros(len1,1);
        sell = zeros(len1,1);
        price = zeros(len1,1);

        count = 1;
        for j = 1:len1

            idChar = data{j,3};
            
            % this is for order id
            id = sscanf(idChar(13:end),'%d');

            if j == 1

                time(count)  = data{j,1};
                buy(count)  = data{j,5};
                sell(count) = data{j,6};
                price(count) = data{j,4};
                dataOverwrite(count,:) = data(j,:);

            elseif j > 1

                %disp('------------------');
                %disp(j);
                %disp(data{j,2});
                %disp([data{j,4} data{j-1,4}]);
                %disp(idPre);
                %disp(id);
                if ((data{j,4} == data{j-1,4}) &&  (idPre == id))
                    buy(count)  = buy(count) + data{j,5};
                    sell(count) = sell(count) + data{j,6};
                    dataOverwrite{count,5} = dataOverwrite{count,5}+data{j,5};
                    dataOverwrite{count,6} = dataOverwrite{count,6}+data{j,6};
                else
                    count = count+1;
                    time(count)  = data{j,1};
                    buy(count)  = data{j,5};
                    sell(count) = data{j,6};
                    price(count) = data{j,4};
                    dataOverwrite(count,:) = data(j,:);
                end

            end

            idPre = id;

        end

        time = time(1:count);
        buy = buy(1:count);
        sell = sell(1:count);
        price = price(1:count);
        dataOverwrite = dataOverwrite(1:count,:);

        
    case 'expand'
        
        len1 = size(data,1);

        time1 = zeros(len1,1);
        buy1 = zeros(len1,1);
        sell1 = zeros(len1,1);
        price1 = zeros(len1,1);
        for j = 1:len1
            time1(j)  = data{j,1};
            buy1(j)  = data{j,5};
            sell1(j) = data{j,6};
            price1(j) = data{j,4};
        end

        len0 = sum(buy1)+sum(sell1);
        
        dataOverwrite = cell(len0,6);

        time = zeros(len0,1);
        buy = zeros(len0,1);
        sell = zeros(len0,1);
        price = zeros(len0,1);

        count = 0;
        for j = 1:len1
            for k = 1:max(buy1(j),sell1(j))
                count = count+1;
                buy(count)  = sign(data{j,5});
                sell(count) = sign(data{j,6});
                price(count) = data{j,4};
                time(count) = data{j,1};
                dataOverwrite(count,:) = data(j,:);
                dataOverwrite{count,5} = sign(data{j,5});
                dataOverwrite{count,6} = sign(data{j,6});
            end
        end

    otherwise
        
        % no action
        
        len1 = length(data);

        time = zeros(len1,1);
        buy = zeros(len1,1);
        sell = zeros(len1,1);
        price = zeros(len1,1);
        
        for j = 1:len1
            time(j)  = data{j,1};
            buy(j)  = data{j,5};
            sell(j) = data{j,6};
            price(j) = data{j,4};
        end
        
        dataOverwrite = data;

end


