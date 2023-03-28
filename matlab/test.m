netpos2 = [3,1,1,0,0,0];
netpos = [2,0,2,0,0,0];
diff = netpos2 - netpos;
scale = 1;
qt = 1;
if scale > 0 && sum(diff<0)>0
   netpos2(diff<0) = netpos(diff<0);
   disp(sum(netpos2 - netpos));
   while sum(netpos2-netpos) ~= scale*qt
       tmp = netpos2-netpos;
       disp(tmp);
       indexJ = find(tmp == max(tmp));
       disp(indexJ(1));
       netpos2(indexJ(1)) = netpos2(indexJ(1))-1;
       disp(netpos2);
   end
   
end

