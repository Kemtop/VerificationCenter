﻿namespace HanumanInstitute.MvvmDialogs.FileSystem;

/// <summary>
/// Manipulates storage items (files and folders) and their contents, and provides information about them
/// </summary>
/// <remarks>
/// This interface inherits <see cref="IDisposable"/> . It's recommended to dispose <see cref="IDialogStorageItem"/> when it's not used anymore.
/// </remarks>
public interface IDialogStorageItem : IDisposable
{
    /// <summary>
    /// Gets the name of the item including the file name extension if there is one.
    /// </summary>
    string Name { get; }

    /// <summary>
    /// Gets the full file-system path of the item, if the item has a path.
    /// </summary>
    /// <remarks>
    /// Android backend might return file path with "content:" scheme.
    /// Browser and iOS backends might return relative uris.
    /// </remarks>
    Uri? Path { get; }

    /// <summary>
    /// Gets a local operating-system representation of a file name.
    /// </summary>
    string LocalPath { get; }

    /// <summary>
    /// Gets the basic properties of the current item.
    /// </summary>
    Task<DialogStorageItemProperties> GetBasicPropertiesAsync();

    /// <summary>
    /// Returns true is item can be bookmarked and reused later.
    /// </summary>
    bool CanBookmark { get; }

    /// <summary>
    /// Saves items to a bookmark.
    /// </summary>
    /// <returns>
    /// Returns identifier of a bookmark. Can be null if OS denied request.
    /// </returns>
    Task<string?> SaveBookmarkAsync();

    /// <summary>
    /// Gets the parent folder of the current storage item.
    /// </summary>
    Task<IDialogStorageFolder?> GetParentAsync();
}

