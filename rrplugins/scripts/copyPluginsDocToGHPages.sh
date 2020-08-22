#! /usr/bin/bash

pluginsRoot="./telPlugins/plugins/released"
ghPluginPagesRoot="./ghPages/docs/plugins"

#plugins="levenberg_marquardt"
plugins="add_noise test_model chisquare levenberg_marquardt nelder_mead monte_carlo_bs auto2000"

#Copy content of html and pdf folders to ghPages
for plugin in $plugins
do
    echo "Copying docs for Plugin $plugin"
    #mkdir $ghPluginPagesRoot/$plugin 
    
    cp -r $pluginsRoot/$plugin/docs/html/* $ghPluginPagesRoot/$plugin/
    cp -r $pluginsRoot/$plugin/docs/pdf/* $ghPluginPagesRoot/$plugin/

done

echo "Done.."


