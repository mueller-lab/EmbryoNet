clc;
close all;
clearvars;

%% Input

sourcepath ='/path2test_data';      % Full path to the experiments
nFrames = 720;  % numebr of frames per time-lapse
time_pattern = ["--LO", "_T"];
nClasses = 8;
Allclasses = ["NORMAL", "NODAL", "BMP", "WNT", "FGF", "SHH", "PCP", "RA", "BOOM", "UNKNOWN", "CUT"];
MainClasses = ["NORMAL", "NODAL", "BMP", "WNT", "FGF", "SHH", "PCP", "RA"];
errorType = 'sem';
initialTime = 2; % 2hpf
timeRate = 2 / 60 ; % time rate of imaging in hours

models          = ["zfish_129__batch_8_000back_Classified_result_1_json" , "zfish_152__batch_8_120back_Classified_result_1_json", "AS_json", "DreadPirateRogers_json"];
modelsCorrected = ["zfish_129__batch_8_000back_Classified_result_1_json_corrected" , "zfish_152__batch_8_120back_Classified_result_1_json_corrected", "AS_json", "DreadPirateRogers_jso"];
modelsNames = ["EmbryoNet", "EmbryoNet - 4h earlier", "Humans"];
allData = cell(length(models),2);



for md = 1: length(models)

    modelName =  models{md};                        %'zfish_129__batch_8_000back_Classified_result_1_json'; %'zfish_152__batch_8_120back_Classified_result_1_json';%'zfish_129__batch_8>
    modelCorrectedName = modelsCorrected{md};      %[modelName '_corrected']; %modelName; %
    GTName = 'GT_000back_json';

    allData{md,1} = modelName;

    %% Collect all data


    detectionsInfo = struct([]); % Structure objtec to collect all data

    experimentsfolders = getallsubfolders(sourcepath);
    Nexp = size(experimentsfolders,1);
    count = 1;

  % Loop for each experiment in the folder
    for exper = 1:Nexp
        disp(['Collecting data from experiment : ' experimentsfolders.name{exper}])
        % get all wells in an experiment
        wellsfolders = getallsubfolders(fullfile(experimentsfolders.folder{exper}, experimentsfolders.name{exper}));
        Nwells = size(wellsfolders,1);
        for well = 1:Nwells

            disp(['     - well : ' wellsfolders.name{well}])
            path_well = fullfile(wellsfolders.folder{well}, wellsfolders.name{well});
            % get path to GT, prediction EmbryoNet, prediction EmbryoNet+Logic

            GT_path = fullfile(path_well, GTName);
            Pred_path = fullfile(path_well, modelName);
            PredCorr_path = fullfile(path_well,modelCorrectedName);

            % get all embryos in the well
            jsonGT_files = struct2table( dir(fullfile(GT_path, '*.json')));
            jsonPred_files = struct2table( dir(fullfile(Pred_path, '*.json')));
            jsonPredCorr_files = struct2table( dir(fullfile(PredCorr_path, '*.json')));

            if size(jsonGT_files,1) ~= nFrames || size(jsonPred_files,1) ~= nFrames ||  size(jsonPredCorr_files,1) ~= nFrames
                continue;
            end

            % sort files by time

            for t = 1: nFrames
                imgName = jsonGT_files.name{t};

                if contains(imgName,time_pattern(1))
                    k = strfind(imgName,time_pattern(1));
                    frame = str2double(imgName(k+4:k+6));
                elseif contains(imgName,time_pattern(2))
                    k = strfind(imgName,time_pattern(2));
                    frame = str2double(imgName(k+2:k+5));
                else
                    frame = [];
                end
              jsonGT_files.frame(t) = frame;
                jsonPred_files.frame(t) = frame;
                jsonPredCorr_files.frame(t) = frame;

            end
            nCols = size(jsonGT_files,2);
            jsonGT_files = sortrows(jsonGT_files,nCols);
            jsonPred_files = sortrows(jsonPred_files,nCols);
            jsonPredCorr_files = sortrows(jsonPredCorr_files,nCols);

            % Get all detections per frame form the corrected model

            Alldetections = cell(nFrames,1);
            AllFoldersGT = jsonGT_files.folder;
            AllFoldersModel = jsonPred_files.folder;
            AllFoldersModelCorrected = jsonPredCorr_files.folder;

            AllNames = jsonPredCorr_files.name;

            parfor t = 1: nFrames
                json_path = fullfile(AllFoldersModelCorrected{t}, AllNames{t});
                json_data = openJsonFile(json_path);
                if ~isempty(json_data.detection_list)
                    if length(json_data.detection_list) == 1
                        detectionsList =  struct2table(json_data.detection_list, "AsArray",true);
                    else
                        detectionsList =  struct2table(json_data.detection_list);
                    end
                    Alldetections{t,1} = detectionsList.id;
                end
            end
            detections = vertcat(Alldetections{:,1});
            detections = unique(detections);

            % get classes from the detections
            ModelClasses = [];
            GTClasses = [];
            t = 3;
           while isempty(ModelClasses)
                Model_json = openJsonFile(fullfile(AllFoldersModel{t}, AllNames{t}));
                ModelClasses = getClassNames(Model_json);
                t = t+1;
            end

            t = 3;
            while isempty(GTClasses)
                GT_json = openJsonFile(fullfile(AllFoldersGT{t}, AllNames{t}));
                GTClasses = getClassNames(GT_json);
                t = t+1;
            end

            ndetections = size(detections,1);
            detInfo = cell(ndetections,1);
            for ii = 1: ndetections
                detInfo{ii}.time = zeros(nFrames, 1);
                detInfo{ii}.time(1) = 1;
                detInfo{ii}.time(2) = 2;
                detInfo{ii}.GT = cell(nFrames, 1);
                detInfo{ii}.PredictionCorrected = cell(nFrames, 1);
                detInfo{ii}.Prediction = cell(nFrames, 1);
                detInfo{ii}.Probability = zeros(nFrames, size(ModelClasses,2));
            end

            for t = 3: nFrames

                GT_json = openJsonFile(fullfile(AllFoldersGT{t}, AllNames{t}));
                Model_json = openJsonFile(fullfile(AllFoldersModel{t}, AllNames{t}));

                if length(GT_json.detection_list) == 1
                    data_GT = struct2table(GT_json.detection_list,"AsArray",true);
                    data_Model = struct2table(Model_json.detection_list,"AsArray",true);
                    data_ModelCorrected = struct2table(openJsonFile(fullfile(AllFoldersModelCorrected{t}, AllNames{t})).detection_list,"AsArray",true);
                else
                    data_GT = struct2table(GT_json.detection_list);
                    data_Model = struct2table(Model_json.detection_list);
                    data_ModelCorrected = struct2table(openJsonFile(fullfile(AllFoldersModelCorrected{t}, AllNames{t})).detection_list);
                end

               for ii = 1: ndetections
                    idInGT = find(data_GT.id == detections(ii));
                    idInModel = find(data_Model.id == detections(ii));
                    idInModelCorrected = find(data_ModelCorrected.id == detections(ii));
                    if  ~isempty(idInGT) && ~isempty(idInModel) &&  ~isempty(idInModelCorrected)
                        if idInGT == idInModel &&  idInGT == idInModelCorrected
                            detInfo{ii}.time(t) = t;
                            detInfo{ii}.GT(t) = data_GT.className(idInGT);
                            detInfo{ii}.PredictionCorrected(t) = data_ModelCorrected.className(idInGT);
                            detInfo{ii}.Prediction(t) = data_Model.className(idInGT);
                            detInfo{ii}.Probability(t,:) = data_Model.prob{idInGT}';
                        else
                            disp(' Different detections in GT and model prediction!!!')
                        end
                    end

                end
            end

            for ii = 1: ndetections
                temp = struct2table(detInfo{ii});

                GT = temp.GT;
                GT(cellfun('isempty',GT)) = [];
                lenTrack = length(GT);
                GT = unique(GT);
                GT(strcmp(GT, 'UNKNOWN')) = [];
                GT(strcmp(GT, 'BOOM')) = [];
                GT(strcmp(GT, 'CUT')) = [];

                Pred = temp.PredictionCorrected;
                Pred(cellfun('isempty',Pred)) = [];
                Pred = unique(Pred);
                Pred(strcmp(Pred, 'UNKNOWN')) = [];
                Pred(strcmp(Pred, 'BOOM')) = [];
                Pred(strcmp(Pred, 'CUT')) = [];
                if ~isempty(GT) && ~isempty(Pred)

                    GTId = find(strcmp(ModelClasses, GT));
                    PredId = find(strcmp(ModelClasses, Pred));

                    ProbabilityGT = temp.Probability(:,GTId);
                    ProbabilityPred = temp.Probability(:,PredId);

                    detectionsInfo(count).ExperimentId = experimentsfolders.name{exper};
                    detectionsInfo(count).wellId = wellsfolders.name{well};
                    detectionsInfo(count).detectionId = detections(ii);
                    detectionsInfo(count).GT = GT{1,1};
                    detectionsInfo(count).Prediction = Pred{1,1};
                    detectionsInfo(count).Length = lenTrack;
                    detectionsInfo(count).time = temp.time';
                    detectionsInfo(count).GTProbablility = ProbabilityGT';
                    detectionsInfo(count).PredictionProbablility = ProbabilityPred';

                    %detectionsInfo(count).data = struct2table(detInfo{ii});


                    count = count+1;
                end
            end
        end
    end
   allData{md,2} = detectionsInfo;

end


learvars -except allData models modelsCorrected sourcepath nFrames time_pattern Allclasses nClasses errorType initialTime timeRate modelsNames MainClasses

%% Merge data from Human annotations

allDataReduced = cell(length(models)-1,2);
for md = 1: length(modelsNames)
    allDataReduced{md,1} = modelsNames{md};
    if md < 3
        allDataReduced{md,2} = allData{md,2};
    else
        allDataReduced{md,2} = [allData{md,2},allData{md+1,2}];
    end
end

clearvars -except allData models modelsCorrected sourcepath nFrames time_pattern Allclasses nClasses errorType initialTime timeRate modelsNames allDataReduced MainClasses

%% Analyze data

% Collect all profiles

allProfiles = cell(nClasses, length(modelsNames)); % Structure objetc to collect all data
allFScores = zeros(nClasses, length(modelsNames));

for md = 1: length(modelsNames)

    modelName =  modelsNames{md};
    disp(['Fitting model : ' modelName])

    % get data
    detectionsInfo = allDataReduced{md,2};
    alldetectionsInfo = struct2table(detectionsInfo);

    % Get complete tracks
    idxCompletTracks = alldetectionsInfo.Length == nFrames-2;
    completeTracks = alldetectionsInfo(idxCompletTracks,:);

    [C,order] = confusionmat(completeTracks.GT,completeTracks.Prediction,'Order',MainClasses);
    stats = statsOfMeasure(C, 1);
    statsClasses = stats.classes;
    allFScores(:,md) = statsClasses(9,:)';
  % Get only the correctly predicted tracks
    idxCorrectClass = cellfun(@strcmp, completeTracks.GT, completeTracks.Prediction);
    completeTracksCorrectlyClass = completeTracks(idxCorrectClass,:);




    for i = 1: nClasses


        idxClass = find(strcmp(completeTracksCorrectlyClass.GT, Allclasses{i}));

        if ~isempty(idxClass)
            tracksClass = completeTracksCorrectlyClass(idxClass,:);
            xAxis= initialTime + (tracksClass.time(1,:) * timeRate );
            GTProbabilities = vertcat(tracksClass.GTProbablility);
            MeanProfile = mean(GTProbabilities,1,'omitnan') ;
            data_std = std(GTProbabilities, 0,1,'omitnan');
            switch errorType
                case 'std', errBar = data_std;
                case 'sem', errBar = (data_std./sqrt(size(GTProbabilities,1)));
                case 'var', errBar = (data_std.^2);
                case 'c95', errBar = (data_std./sqrt(size(GTProbabilities,1))).*1.96;
            end
            if i ==3
                [~,b] = max(MeanProfile);
                [fitresult, gof] = FitSigmoid(xAxis(1:b), MeanProfile(1:b));
            else
                [fitresult, gof] = FitSigmoid(xAxis, MeanProfile);
            end
            % get time for 50% probablitity
            profile50 = abs(MeanProfile-0.5);
            [~,idx] = min(profile50);
            mtime = xAxis(idx);

      else
            MeanProfile = nan(1,nFrames);
            errBar = nan(1,nFrames);
            fitresult = [];
            gof = [];
            mtime = 0;
        end


        % Collect the data

        Data.xAxis = xAxis;
        Data.profile = MeanProfile;
        Data.errBar = errBar;
        Data.fit = fitresult;
        Data.gof = gof;
        Data.mtime = mtime;
        allProfiles{i,md} = Data;

    end

end

clearvars -except allData models modelsCorrected sourcepath nFrames time_pattern Allclasses nClasses errorType initialTime timeRate allProfiles modelsNames allDataReduced MainClasses >


%% Plot data
%cmap=num2cell(hsv(length(modelsNames)),2);
cmap = {[232,183,166]/255; [35,166,209]/255; [57,165,58]/255};
st = {'d', '<', 'o'};
for i = 1: nClasses
    figure(i)

    title(Allclasses(i))
    xlabel('time [hpf]')
    ylabel('Classification probability')
    for md = 1: length(modelsNames)
        Data = allProfiles{i,md};
        xAxis = Data.xAxis;
        MeanProfile = Data.profile;
        errBar = Data.errBar;

        lineProps.col = {cmap{md}};
        lineProps.style = '-';
        lineProps.marker = st{md};
        mseb(xAxis, MeanProfile, errBar,lineProps,0);
        hold on

        %h = plot( Data.fit, xAxis, MeanProfile, 'predobs', 0.95 );
        %pause
    end

    for md = 1: length(modelsNames)
        Data = allProfiles{i,md};
        xAxis = Data.xAxis;
        param = Data.fit;
        yModel = param.b ./ (1+exp(-param.c*(xAxis-param.a)));
        plot(xAxis, yModel, 'Color', cmap{md})
    end

    legend(modelsNames, 'Location', 'best')
    xlim([2 26])
    ylim([0 1.02])
    fileOut = Allclasses(i);
    set(gca,'TickDir','out');
    set(gcf, 'Position', [2062 229 1666 1023])
   saveas(gcf,fileOut,'svg')
    saveas(gcf,fileOut,'fig')
    close all
end


estTimes = zeros(nClasses, length(modelsNames));
mTimes = zeros(nClasses, length(modelsNames));
estTimesUp = zeros(nClasses, length(modelsNames));
estTimesDown = zeros(nClasses, length(modelsNames));

for i = 1: nClasses
    for md = 1: length(modelsNames)
        Data = allProfiles{i,md};
        fitresult = Data.fit;
        confInt = confint(fitresult);
        if ~isempty(fitresult)
            estTimes(i,md) = fitresult.a;
            estTimesUp(i,md) = confInt(2,1);
            estTimesDown(i,md) = confInt(1,1);
            mTimes(i,md) = Data.mtime;
        end
    end
end



OrderedClasses = ["NORMAL", "BMP", "RA", "WNT", "FGF", "NODAL", "SHH", "PCP"];
%MainClasses = ["NORMAL", "NODAL", "BMP", "WNT", "FGF", "SHH", "PCP", "RA"];
OrderIdx = [1, 3, 8, 4, 5, 2, 6, 7 ];
simb = ['d', '<', 'o'];
y = 1:8;
figure
for md = 1: length(modelsNames)
    x = estTimes(OrderIdx,md);
    %xUp = estTimesUp(OrderIdx,md) - x;
    %xDown= x - estTimesDown(OrderIdx,md);
    %errorbar(x,y,xDown,xUp, '.', 'horizontal','Color', cmap{md})
    scatter(x,y, simb(md), 'filled','Color', cmap{md})
    hold on

end
xlim([6,22])
yticklabels(OrderedClasses)
ylim([0,9])
%legend(modelsNames)
xlabel("Time[hpf]")
saveas(gcf,"allTimes_fitting",'eps')
saveas(gcf,"allTimes_fitting",'fig')

close all


figure
for md = 1: length(modelsNames)
    x = mTimes(OrderIdx,md);
    scatter(x,y, simb(md), 'filled','Color', cmap{md})
    hold on

end
xlim([6,22])
yticklabels(OrderedClasses)
ylim([0,9])
%legend(modelsNames)
xlabel("Time[hpf]")
saveas(gcf,"allTimes_50",'eps')
saveas(gcf,"allTimes_50",'fig')
close all


% color for classes

allColors = ["#000000", "EF3B34", "462F86", "606261", "2F67B1", "2F8B43", "F58330", "F6E333"];


for i = 1: nClasses
    allColorsRGB{i} =  hex2rgb(allColors{i},255);
end
% fscore vs time




for md = 1: length(modelsNames)
    x0 = estTimes(OrderIdx,md);
    y0 = allFScores(OrderIdx,md);

    for i = 1: nClasses
        x = x0(i);
        y = y0(i);
        colori = hex2rgb(allColors{i},255);
        scatter(x,y,simb(md), 'filled','Color', allColors{i});
        hold on
    end
end


