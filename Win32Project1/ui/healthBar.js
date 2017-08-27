var game=new Phaser.Game(300, 70, Phaser.WEBGL, 'gameDiv', { preload: preload, create: create, update: update,render: render },true);

function preload() {

    game.load.image('healthBarBack','myUI/healthBarBack.png');
    game.load.image('healthBarFore','myUI/healthBarFore.png');

}

var barBack;
var barFore;

var buttonPlay;

var tweenX;
function create() {
    tweenX=1;
    
    barBack=game.add.sprite(0,0,'healthBarBack');
    barFore=game.add.sprite(30,16,'healthBarFore');
}

function damage(dmg)
{
    if(tweenX.toFixed(1)<=0.0001) 
    {
        return;
    }
    
    tweenX-=dmg;

    if(tweenX.toFixed(1)<=0.0001) 
    {
        tweenX=0.0;
    }

   	game.add.tween(barFore.scale).to( { x: tweenX }, 500, Phaser.Easing.Bounce.Out, true);

}

function update() 
{	
}

function render()
{
    game.debug.text( tweenX.toFixed(1), 100, 380 );
}