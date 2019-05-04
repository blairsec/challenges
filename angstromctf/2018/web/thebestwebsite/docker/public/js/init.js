/*
	Ion by TEMPLATED
	templated.co @templatedco
	Released for free under the Creative Commons Attribution 3.0 license (templated.co/license)
*/

ids = ["598f82efd8122b1af03432bc","598f82efd8122b1af03432bd","598f82efd8122b1af03432be"];

(function($) {

	skel.init({
		reset: 'full',
		breakpoints: {

			// Global.
				global: {
					range: '*',
					href: 'css/style.css',
					containers: 1400,
					grid: {
						gutters: {
							vertical: '4em',
							horizontal: 0
						}
					}
				},

			// XLarge.
				xlarge: {
					range: '-1680',
					href: 'css/style-xlarge.css',
					containers: 1200
				},

			// Large.
				large: {
					range: '-1280',
					href: 'css/style-large.css',
					containers: 960,
					grid: {
						gutters: {
							vertical: '2.5em'
						}
					},
					viewport: {
						scalable: false
					}
				},

			// Medium.
				medium: {
					range: '-980',
					href: 'css/style-medium.css',
					containers: '90%',
					grid: {
						collapse: 1
					}
				},

			// Small.
				small: {
					range: '-736',
					href: 'css/style-small.css',
					containers: '90%',
					grid: {
						gutters: {
							vertical: '1.25em'
						}
					}
				},

			// XSmall.
				xsmall: {
					range: '-480',
					href: 'css/style-xsmall.css',
					grid: {
						collapse: 2
					}
				}

		},
		plugins: {
			layers: {

				// Config.
					config: {
						transform: true
					},

				// Navigation Panel.
					navPanel: {
						animation: 'pushX',
						breakpoints: 'medium',
						clickToHide: true,
						height: '100%',
						hidden: true,
						html: '<div data-action="moveElement" data-args="nav"></div>',
						orientation: 'vertical',
						position: 'top-left',
						side: 'left',
						width: 250
					},

				// Navigation Button.
					navButton: {
						breakpoints: 'medium',
						height: '4em',
						html: '<span class="toggle" data-action="toggleLayer" data-args="navPanel"></span>',
						position: 'top-left',
						side: 'top',
						width: '6em'
					}

			}
		}
	});

	$(function() {

		$.ajax({
			url: "/boxes?ids="+ids.join(","),
			success: function(data) {
				data = JSON.parse(data)
				$("#box1_title").text(data.boxes[0].data.split("^")[0])
				$("#box1_caption").text(data.boxes[0].data.split("^")[1])
				$("#box2_title").text(data.boxes[1].data.split("^")[0])
				$("#box2_caption").text(data.boxes[1].data.split("^")[1])
				$("#box3_title").text(data.boxes[2].data.split("^")[0])
				$("#box3_caption").text(data.boxes[2].data.split("^")[1])
			}
		})

	});

})(jQuery);
