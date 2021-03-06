
import { GUIApplication, Root, Div, Scroll, New, CSS } from 'ngui';
import { Color } from 'ngui/value';
import { random } from 'ngui/util';
import './uu';

CSS({
	'.item': {
		height: 40,
		width: '25%',
	},
})

new GUIApplication({ multisample: 2 }).start(
	<Root backgroundColor="#000" />
).onLoad = function() {

	uu.start();

	New(
		<Scroll width="full" height="full">
			${
				Array.from({ length: 10000 }, ()=>{
					var color = new Color(random(0, 255), 
						random(0, 255), random(0, 255), 255);
					return <Div backgroundColor=color class="item" />;
				})
			}
		</Scroll>
	).appendTo(this.root);

	uu.log();

};

uu.show_fsp();
