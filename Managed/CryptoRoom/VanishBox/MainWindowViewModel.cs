using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Reflection;
using System.Threading.Tasks;
using System.Windows.Input;
using HanumanInstitute.MvvmDialogs;
using HanumanInstitute.MvvmDialogs.FrameworkDialogs;
using ReactiveUI;
using IOPath = System.IO.Path;

namespace VanishBox;

public class MainWindowViewModel : ViewModelBase
{
    private readonly IDialogService _dialogService;

    public ObservableCollection<string> FilesPaths { get; private set; } = new();
    public ICommand SelectFilesCommand { get; }
    public ICommand ResetSettingsCommand { get; }

    private string _confirmation = string.Empty;
    public string Confirmation
    {
        get => _confirmation;
        private set => this.RaiseAndSetIfChanged(ref _confirmation, value, nameof(Confirmation));
    }

    public MainWindowViewModel(IDialogService dialogService)
    {
        this._dialogService = dialogService;
        SelectFilesCommand = ReactiveCommand.Create(SelectFilesAsync);
        ResetSettingsCommand = ReactiveCommand.Create(ResetSettingsAsync);;
    }
    
    private async Task SelectFilesAsync()
    {
        var settings = GetSettings(true);
        var result = await _dialogService.ShowOpenFilesDialogAsync(this, settings);
        FilesPaths.Clear();
        foreach (var item in result)
        {
            FilesPaths.Add(item?.Path?.LocalPath ?? string.Empty);
        }
    }

    private OpenFileDialogSettings GetSettings(bool multiple) => new OpenFileDialogSettings
    {
        Title = multiple ? "Выберите файлы" : "Выберите файл",
        InitialDirectory = IOPath.GetDirectoryName(Assembly.GetExecutingAssembly().Location)!,
        Filters = new List<FileFilter>()
        {
            new FileFilter(
                "Text Documents",
                new[]
                {
                    "*", "all"
                }),
            new FileFilter(
                "Binaries",
                new[]
                {
                    ".exe", ".dll"
                }),
            new FileFilter("All Files", "*")
        }
    };

    private async Task ResetSettingsAsync()
    {
        var result = await _dialogService.ShowMessageBoxAsync(
            this,
            "",
            "");
    }
}
