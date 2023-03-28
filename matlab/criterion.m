function mae = criterion(pnlAcctSoFar, cumPnl, af, option)

switch option
    
    case '0'
        
        mae = 0;
        nAccts = length(pnlAcctSoFar);
        for i = 1:nAccts
            for j = 1:nAccts
                if i~=j
                    mae = mae+(pnlAcctSoFar(i)/af(i)-pnlAcctSoFar(j)/af(j))^2;
                end
            end
        end       
    
    case '1'
        
        mae = sum((pnlAcctSoFar./af - cumPnl).^2);
        
    case '2'
        
        mae = sum(abs((pnlAcctSoFar - af.*cumPnl)./af.*cumPnl));
        
    case '3'
        
        mae = sum((pnlAcctSoFar - af.*cumPnl).^2);
        
    case '4'
        
        mae = sum(abs(pnlAcctSoFar - af.*cumPnl));
        
end