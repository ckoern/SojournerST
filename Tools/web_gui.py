import time 

import numpy as np

from bokeh.models.sources import ColumnDataSource
from bokeh.plotting import figure, curdoc
from bokeh.layouts import column, row, layout, widgetbox, Spacer


from bokeh.models import  TextInput, Button

from packet import *


data_source = ColumnDataSource()
start_time = time.time()

def create_input(name):
    input = TextInput(value="", title=name)
    btn = Button(label = 'send', button_type = "warning", width = 100)

    return row(input, btn)

def create_read_figure( var ):
    fig = figure(width=400, height=400)
    fig.line(x="time", y=var[0], source = data_source, line_width=2)
    return fig
variables = ["Target CPS", "Current CPS"]

plot_variables = [ ("Current CPS", ChannelCommandType.channel_current_cps),
                   ("Target CPS", ChannelCommandType.channel_get_target_cps),
                   ("Setpoint Error", ChannelCommandType.channel_pid_setpoint_error),
                   ("PID Integrator", ChannelCommandType.channel_pid_integrator_state ),
                   ("PID Differential", ChannelCommandType.channel_pid_filter_state)
 ]


def poll_data():
    dt = time.time() - start_time
    new_data = { v: [np.random.randint(0,100)] for v,vi in plot_variables }
    new_data["time"] = [dt ]

    data_source.stream(new_data)

empty_data =  { v: [] for v, vc in plot_variables }
empty_data["time"] = []
data_source.data =empty_data
rows = [ create_input(v) for v in variables ]

col = column(*rows)
figs = column( [create_read_figure(v) for v in plot_variables] )

top_layput = row(col, figs) 


curdoc().add_root(top_layput)

curdoc().add_periodic_callback( poll_data, 1000 )
