`timescale 1 ps / 1 ps

//触摸笔采样模块，保证触摸中断的稳定性，实现异步路径的安全传输

module touchPenSmp(
	input clk_100M,
	input rst,
	input [15:0] countMax,//调整采样周期
	input touchPenIn,
	output reg touchPenOut
	);
	
	reg [23:0] count1;
	
	always @( posedge clk_100M or negedge rst )
	begin
		if ( !rst )
		begin
			count1 <= 1'b0;
			touchPenOut <= 1'b1;
		end		
		else if ( count1 > { countMax[9:0], 14'b00_0000_0000_0000 } )
		begin
			touchPenOut <= touchPenIn;//低速率采样输出
			count1 <= 24'h2;
		end
		else
		begin
			count1 <= count1 + 1'b1;
		end
	end
	
	
endmodule
