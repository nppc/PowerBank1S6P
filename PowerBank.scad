$fn=40;
//Batteries(18.2,65);
//translate([-43.5,-34.5,0])TP4056();
//translate([40,-32,0])StepUp();
//translate([0,-33,-2])SwitchBoard();


//difference(){
//    mainBox();
//    translate([-1.5,-37.5,4])screwSocket();
//    translate([17.5,-37.5,4])screwSocket();

    difference(){
      translate([0,-1.5,9.2])topCover();
      Batteries(19,67);
      translate([-1.5,-37.5,4])cylinder(d=1.4, h=10, center=true,$fn=20);
      translate([17.5,-37.5,4])cylinder(d=1.4, h=10, center=true,$fn=20);
      translate([40,-32,0])StepUp();
      translate([-43.5,-34.5,0])TP4056();
      translate([0,0,10.26])linear_extrude(height = 0.2)
        text("6K", size = 25, font = "Arial Black", halign = "center", valign = "center", $fn = 40);

    }

  //translate([-100,-50,-100])cube([100,100,100]);
//}

module mainBox(){
  difference(){
      translate([0,-1.5,0])box(118,82,21,3,1.3);
      translate([8,-37,10.5])cutOutSW();
      translate([-43.5,-34.5,0])TP4056();
      translate([0,-33,-2])SwitchBoard();
      Batteries(19,67);
      //translate([50,-32-4.2,0])cube([20,6.8,13.2],true);//stepup
      translate([55,-32-4.2,0])cube([10,7.5,14.5],true);//stepup
      // vent holes
      for(i1=[0:4:15])for(i=[0:5:40])translate([-50+i,-37,6-i1])cube([1,10,1],true);
      for(i1=[0:4:15])for(i=[0:5:20])translate([24+i,-37,6-i1])cube([1,10,1],true);
  }
  translate([-56.5,36.8,21/2-1.5])rotate([0,0,-90])bordjur(113);
  difference(){
    translate([56.5,-39.8,21/2-1.5])rotate([0,0,90])bordjur(113);
    translate([8,-37,10])cutOutSW();
    translate([8,-38,6.6])cube([30,5,5],true);
  }
  difference(){
    translate([-56.3,-40,21/2-1.5])bordjur(77);
    translate([-56.4,-36,21/2-5.4])cube([3,3,4]);
  }
  translate([56.3,37,21/2-1.5])rotate([0,0,180])bordjur(77);
}

module TP4056(){
    color("BLUE")cube([26,1.6,19],true);
    color("GRAY")translate([-10,-2.8,0])cube([9.2,4.3,7.8],true);
    color("RED")translate([3,-1,7])cube([1,1,2],true);
    color("GREEN")translate([6,-1,7])cube([1,1,2],true);
}

module StepUp(){
    color("BLUE")cube([34,1.6,16],true);
    color("GRAY")translate([10,-4.2,0])cube([14.5,6.8,13.2],true);
    color("RED")translate([-10.5,-1,7-3.5])cube([1,1,2],true);
}

module SwitchBoard(){
    color("GREEN")cube([37,2,15],true);
    color("GRAY")translate([8,-3.7,5])cube([13,5.5,7],true);
    color("BLACK")translate([8+1.5,-3.7,10.5])cube([3,3,4],true);
}

module box(x,y,z,d,wall){
    translate([0,0,-z/2])linear_extrude(height=z){
        difference(){
            hull(){
                translate([x/2-d,y/2-d])circle(d=d,$fn=25);
                translate([x/2-d,-y/2+d])circle(d=d,$fn=25);
                translate([-x/2+d,-y/2+d])circle(d=d,$fn=25);
                translate([-x/2+d,y/2-d])circle(d=d,$fn=25);
            }
            d1=d-wall*2;
            hull(){
                translate([x/2-d,y/2-d])circle(d=d1,$fn=10);
                translate([x/2-d,-y/2+d])circle(d=d1,$fn=10);
                translate([-x/2+d,-y/2+d])circle(d=d1,$fn=10);
                translate([-x/2+d,y/2-d])circle(d=d1,$fn=10);
            }
        }
    }
    translate([0,0,-z/2])linear_extrude(height=1.5)hull(){
        translate([x/2-d,y/2-d])circle(d=d,$fn=25);
        translate([x/2-d,-y/2+d])circle(d=d,$fn=25);
        translate([-x/2+d,-y/2+d])circle(d=d,$fn=25);
        translate([-x/2+d,y/2-d])circle(d=d,$fn=25);
    }


    // for TP
    translate([-43,-33,-z/2+1.5])linear_extrude(height=1)square([27,4],true);
    // for SW
    translate([0,-31.5,-z/2+1.5])linear_extrude(height=1)
    square([39,5],true);
    //stepup
    translate([22,-32.5,-z/2+1.5])cube([35,4,0.8]);
    translate([46.5,-30.5,-z/2+1.5+0.8])linear_extrude(height=1)difference(){
        square([20,4],true);
        square([30,1.6],true);
    }
}

module cutOutSW(){
    rotate([90,0,0])linear_extrude(height=5)hull(){
        square([24,5],true);
        translate([0,4])square([24,5],true);
    }
}

module Batteries(bd,hg){
    for(i=[-18.5*2.5:18.5:18.5*2.5])translate([i,3,0])rotate([90,0,0])color("LIGHTBLUE")cylinder(d=bd,h=hg,center=true);
}

module screwSocket(){
  difference(){
    hull(){
      cube([5,5,5],true);
      translate([0,-2.5,-4])cube([5,1,5],true);
    }
    cylinder(d=1,h=8,center=true,$fn=10);
  }
}

module bordjur(l){
  rotate([-90,0,0])linear_extrude(height=l)polygon(points=[[0,0],[1,0],[0,3]]);
  rotate([-90,0,0])linear_extrude(height=l)polygon(points=[[0,0],[0,-1.5],[0.5,-1.5]]);
}

module topCover(){
  difference(){
    hull(){
      cube([112,76,0.1],true);
      translate([0,0,1.2])cube([112-1,76-1,0.1],true);
    }
    translate([8,-36,0])cube([24,10,5],true);
  }
  
  difference(){
    translate([8,-34,-1.9])cube([24,8,1.4],true);
    translate([8,-36,0])cube([7,7,50],true);//switch
  }

  translate([8,-30.25,-1.1])cube([24,1.5,3],true);
  
  translate([-5.5,-34,-1.1])cube([3,8,3],true);
  translate([21.5,-34,-1.1])cube([3,8,3],true);
  translate([44,-30.5,-1])cube([20,4,1],true);//stepup
  translate([-42,-33,0])cube([25,4,1],true);//TP4056
  
  translate([112/2,76/2,1.05])cylinder(d=12,h=0.2);
  translate([112/2,-76/2,1.05])cylinder(d=12,h=0.2);
  translate([-112/2,-76/2,1.05])cylinder(d=12,h=0.2);
  translate([-112/2,76/2,1.05])cylinder(d=12,h=0.2);

}