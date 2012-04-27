{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 314.0, 164.0, 272.0, 310.0 ],
		"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
		"bglocked" : 0,
		"defrect" : [ 314.0, 164.0, 272.0, 310.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 14.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 0,
		"boxanimatetime" : 200,
		"imprint" : 1,
		"enablehscroll" : 0,
		"enablevscroll" : 0,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"checkedcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bordercolor" : [ 0.5, 0.5, 0.5, 1.0 ],
					"outlettype" : [ "int" ],
					"background" : 0,
					"patching_rect" : [ 90.0, 255.0, 20.0, 20.0 ],
					"presentation" : 0,
					"ignoreclick" : 0,
					"id" : "obj-7",
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route locked",
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"fontface" : 0,
					"fontsize" : 14.0,
					"numoutlets" : 2,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"outlettype" : [ "", "" ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"patching_rect" : [ 90.0, 210.0, 87.0, 23.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"presentation" : 0,
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"id" : "obj-4",
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sel 1",
					"numinlets" : 2,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"fontface" : 0,
					"fontsize" : 14.0,
					"numoutlets" : 2,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"outlettype" : [ "bang", "" ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"patching_rect" : [ 30.0, 120.0, 40.0, 23.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"presentation" : 0,
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"id" : "obj-2",
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "active",
					"numinlets" : 0,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"fontface" : 0,
					"fontsize" : 14.0,
					"numoutlets" : 1,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"outlettype" : [ "int" ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"patching_rect" : [ 30.0, 90.0, 47.0, 23.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"presentation" : 0,
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"id" : "obj-3",
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "fpic",
					"autofit" : 0,
					"numinlets" : 1,
					"alpha" : 1.0,
					"numoutlets" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"background" : 0,
					"yoffset" : 0.0,
					"patching_rect" : [ 195.0, 225.0, 398.0, 227.0 ],
					"pic" : "gooegg.jpg",
					"presentation" : 0,
					"ignoreclick" : 0,
					"xoffset" : 0.0,
					"id" : "obj-8",
					"hidden" : 0,
					"embed" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"bgcolor" : [ 0.91328, 0.91328, 0.91328, 0.75 ],
					"numoutlets" : 1,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"outlinecolor" : [ 0.71328, 0.71328, 0.71328, 1.0 ],
					"outlettype" : [ "bang" ],
					"background" : 0,
					"patching_rect" : [ 22.0, 217.0, 20.0, 20.0 ],
					"presentation" : 0,
					"ignoreclick" : 0,
					"fgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"id" : "obj-5",
					"blinkcolor" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "quebec",
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"fontface" : 0,
					"fontsize" : 14.0,
					"numoutlets" : 2,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"outlettype" : [ "bang", "" ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"patching_rect" : [ 45.0, 180.0, 57.0, 23.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"presentation" : 0,
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"id" : "obj-1",
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Québec",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"fontface" : 3,
					"fontsize" : 20.871338,
					"numoutlets" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"background" : 0,
					"patching_rect" : [ 12.0, 12.0, 234.0, 30.0 ],
					"underline" : 0,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"presentation" : 0,
					"fontname" : "Arial",
					"ignoreclick" : 1,
					"id" : "obj-14",
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"mode" : 1,
					"bgcolor" : [ 0.66667, 0.66667, 0.66667, 1.0 ],
					"grad1" : [ 0.188235, 0.105882, 0.266667, 1.0 ],
					"border" : 0,
					"shadow" : 0,
					"numoutlets" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"grad2" : [ 0.858824, 0.858824, 0.858824, 1.0 ],
					"angle" : 0.0,
					"background" : 1,
					"patching_rect" : [ 9.0, 8.0, 253.0, 62.0 ],
					"presentation" : 0,
					"ignoreclick" : 1,
					"rounded" : 8,
					"id" : "obj-23",
					"hidden" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
 ]
	}

}
